--[[----------------------------------------------------------------------------
  premake4.lua -- Noel Cower
  This build script is public domain.
------------------------------------------------------------------------------]]


-- Set up snow table
snow = {}

--[[----------------------------------------------------------------------------
  join_arrays

    Joins zero or more arrays into a single array. Discards original keys in
    input tables.
------------------------------------------------------------------------------]]
function snow.join_arrays(...)
  local result = {}
  local inputs = {...}
  for ik,iv in pairs(inputs) do
    for jk,jv in pairs(iv) do
      result[#result + 1] = jv
    end
  end
  return result
end


function snow.tostring(e)
  if e == nil then
    return ""
  elseif type(e) == "function" then
    return snow.tostring(e())
  elseif type(e) == "number" then
    return tostring(e)
  elseif type(e) == "boolean" then
    if e then
      return "1"
    else
      return "0"
    end
  else
    return tostring(e)
  end
end


--[[----------------------------------------------------------------------------
  formatrb_string

    Returns arguments combined for format.rb
------------------------------------------------------------------------------]]
function snow.formatrb_string(t)
  local result = ""
  for k,v in pairs(t) do
    result = result .. " '" .. snow.tostring(k) .. "=" .. snow.tostring(v) .. "'"
  end
  return result
end


--[[----------------------------------------------------------------------------
  get_headers

    Retrieves a list of all headers, excluding any that match the suffixes in
    the excluded_suffixes argument (if provided).
------------------------------------------------------------------------------]]
function snow.get_headers(excluded_suffixes)
  local headers = snow.join_arrays(
    os.matchfiles("include/**.hh"),
    os.matchfiles("include/**.cc"),
    os.matchfiles("include/*.hh"),
    os.matchfiles("include/*.cc") )
  local swap = {}
  for header_k, header_path in pairs(headers) do
    if excluded_suffixes then
      -- Check for exclusions
      for excl_k, excl_suffix in pairs(excluded_suffixes) do
        if string.endswith(header_path, excl_suffix) then
          print(excl_k .. ": Excluding " .. header_path)
          header_path = nil
          break
        end -- if matches excluded header
      end -- for in excluded_suffixes
    end -- if excluded_suffixes

    if header_path then
      swap[header_path] = header_path
    end
  end -- for in headers
  return swap
end


function snow.install(source_path, install_path, verbose)
  if verbose then
    print("Installing '" .. source_path .."' to '" .. install_path .. "'")
  end
  local done, errmsg = os.copyfile(source_path, install_path)
  if not done then
    print("os.copyfile: " .. errmsg)
    print("Unable to copy " .. source_path, " unable to continue installation")
    return false
  end
  return true
end


function snow.correct_dirsep(str)
  if os.is("windows") then
    return string.gsub(str, "/", "\\")
  else
    return string.gsub(str, "\\", "/")
  end
end


function snow.dirname(str)
  local last = string.findlast(str, "/", true)
  if last ~= nil then
    return string.sub(str, 0, last)
  end
  return str
end


function snow.mkdir_p(path)
  local dirsep = "/"
  if os.is("windows") then
    dirsep = "\\"
  end

  local components = string.explode(path, "/")
  local last_dir = 0

  do -- Make sure nothing along the way is a file
    local dir_build = ""
    for i, v in ipairs(components) do
      if v ~= "" then
        dir_build = dir_build .. dirsep .. v
        if os.isdir(dir_build) == true then
          last_dir = i
        elseif os.isfile(dir_build) then
          return false -- Cannot create dir if file of the same name exists already
        end
      end
    end -- for in components
  end -- do check path

  do
    local dir_build = ""
    for i, v in ipairs(components) do
      if #v > 0 then
        dir_build = dir_build .. dirsep .. v
        if i > last_dir then
          local done, errmsg = os.mkdir(dir_build)
          if not done then
            print("Unable to create directory: " .. dir_build)
            return false
          end -- if not done
        end -- if i > last_dir
      end
    end -- for in components
  end -- do build directory

  return true
end -- mkdir_p



-- Main build configuration

solution "snow"
project "snow-common"
configurations { "Debug-Static", "Debug-Shared", "Release-Static", "Release-Shared" }
targetdir "lib"

-- Option defaults
g_verbose_install = false
g_exclude_suffixes = {}
g_prefix = "/usr/local"

if _OPTIONS["verbose-install"] then
  g_verbose_install = true
end

if _OPTIONS["prefix"] then
  g_prefix = _OPTIONS["prefix"]
end

g_libdir        = g_prefix .. "/lib"
g_pkgconfig_dir = g_libdir .. "/pkgconfig"

-- Build options
newoption {
  trigger = "verbose-install",
  description = "Provides verbose information during installation"
}

newoption {
  trigger = "universal",
  description = "Builds for x86_64 and i386 if enabled. Otherwise builds for native architecture. [OS X only]"
}

newoption {
  trigger = "no-exceptions",
  description = "Disables exceptions in snow-common -- replaces throws with exit(1)"
}

newoption {
  trigger = "prefix",
  description = "Installation prefix",
  value = "Default: /usr/local"
}

-- Installation
newaction {
  trigger = "install",
  description = "Install libsnow-common",
  execute = function()
    local headers = snow.get_headers(g_exclude_suffixes)
    local dirs_needed = {}
    for k,v in pairs(headers) do
      local dir = snow.dirname(v)
      if not dirs_needed[dir] then
        dirs_needed[dir] = true
      end
    end

    -- Make sure prefix/lib/pkgconfig exists
    dirs_needed[g_pkgconfig_dir] = g_pkgconfig_dir

    -- Create directories
    print("Creating directories...")
    for k,v in pairs(dirs_needed) do
      if not os.isdir(k) then
        if g_verbose_install then
          print("Creating directory " .. v)
        end

        if not snow.mkdir_p(g_prefix .. "/" .. k) then
          print("Could not create directories needed, unable to continue installation")
          return false
        end
      end
    end

    -- Copy headers over
    print("Copying headers...")
    for k,v in pairs(headers) do
      local source_path = snow.correct_dirsep(v)
      local install_path = snow.correct_dirsep(g_prefix .. "/" .. v)
      if not snow.install(source_path, install_path, g_verbose_install) then
        return false
      end
    end

    print("Copying libraries...")
    local lib_files = os.matchfiles("lib/libsnow-common.*")
    for k,v in pairs(lib_files) do
      local source_path = snow.correct_dirsep(v)
      local install_path = snow.correct_dirsep(g_prefix .. "/" .. v)
      if not snow.install(source_path, install_path, g_verbose_install) then
        return false
      end
    end

    print("Copying 'snow-common.pc'...")
    if not snow.install("snow-common.pc", g_pkgconfig_dir .. "/snow-common.pc", g_verbose_install) then
      return false
    end
  end
}

-- Compiler flags
g_version = "0.1.0"
language "C++"
buildoptions { "-std=c++11" }
flags { "FloatStrict", "NoRTTI" }
objdir "obj"

-- Add sources/include directories
includedirs { "include" }
files { "src/**.cc" }

configuration "*-Shared"
kind "SharedLib"

configuration "*-Static"
kind "StaticLib"

configuration "Release-*"
defines { "NDEBUG" }

configuration "Debug-*"
defines { "DEBUG" }
flags { "Symbols" }

g_build_config_opts = {
  USE_EXCEPTIONS = not _OPTIONS["no-exceptions"],
  HAS_SHA256 = false,
  HAS_LBIND = false
}

g_pkgconfig_opts = {
  PREFIX = g_prefix,
  VERSION = g_version,
  PRIVATE_PKGS = "",
  PRIVATE_LIBS = ""
}

-- Exceptions
configuration "no-exceptions"
flags { "NoExceptions" }

-- OS X specific options
configuration { "macosx", "universal" }
buildoptions { "-arch x86_64", "-arch i386" }
linkoptions { "-arch x86_64", "-arch i386" }

configuration "macosx"
buildoptions { "-stdlib=libc++" }
links { "c++" }

configuration { "macosx", "*-Shared" }
links { "Cocoa.framework" }

configuration {}

-- Generate build-config/pkg-config
local config_src = "'include/snow/build-config.hh.in'"
local config_dst = "'include/snow/build-config.hh'"

if _ACTION and _ACTION ~= "install" then
  -- Generate build-config.hh
  print("Generating 'include/snow/build-config.hh'...")
  os.execute("./format.rb " .. config_src .. " " .. config_dst .. snow.formatrb_string(g_build_config_opts))

  -- Generate snow-common.pc
  print("Generating 'snow-common.pc'...")
  local formatrb_exec = "./format.rb 'snow-common.pc.in' 'snow-common.pc'" .. snow.formatrb_string(g_pkgconfig_opts)

  if os.execute(formatrb_exec) ~= 0 then
    print("Could not create 'snow-common.pc', unable to continue installation")
    return false
  end
end
