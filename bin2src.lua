--
-- Copyright 2010-2017 Branimir Karadzic. All rights reserved.
-- License: https://github.com/bkaradzic/bx#license-bsd-2-clause
--

project "bin2c"
	kind "ConsoleApp"

	includedirs {
		bx_includedirs,
		fmtlib_includedirs,
	}

	files {
		"./bin2c/**.cpp",
		"./bin2c/**.h",
	}

	links {
		"bx",
		"fmtlib",
	}

	configuration { "mingw-*" }
		targetextension ".exe"

	configuration {}

	strip()

---

project "bin2cpparr"
	kind "ConsoleApp"

	includedirs {
		bx_includedirs,
		fmtlib_includedirs,
	}

	files {
		"./bin2cpparr/**.cpp",
		"./bin2cpparr/**.h",
	}

	links {
		"bx",
		"fmtlib",
	}

	configuration { "mingw-*" }
		targetextension ".exe"

	configuration {}

	strip()

---

project "bin2cppvec"
	kind "ConsoleApp"

	includedirs {
		bx_includedirs,
		fmtlib_includedirs,
	}

	files {
		"./bin2cppvec/**.cpp",
		"./bin2cppvec/**.h",
	}

	links {
		"bx",
		"fmtlib",
	}

	configuration { "mingw-*" }
		targetextension ".exe"

	configuration {}

	strip()

---

project "bin2cppstring"
	kind "ConsoleApp"

	includedirs {
		bx_includedirs,
		fmtlib_includedirs,
	}

	files {
		"./bin2cppstring/**.cpp",
		"./bin2cppstring/**.h",
	}

	links {
		"bx",
		"fmtlib",
	}

	configuration { "mingw-*" }
		targetextension ".exe"

	configuration {}

	strip()

---

project "bin2cppu64vec"
	kind "ConsoleApp"

	includedirs {
		bx_includedirs,
		fmtlib_includedirs,
	}

	files {
		"./bin2cppu64vec/**.cpp",
		"./bin2cppu64vec/**.h",
	}

	links {
		"bx",
		"fmtlib",
	}

	configuration { "mingw-*" }
		targetextension ".exe"

	configuration {}

	strip()

---


---
--- variants relying on incbin to do the embedding
--- see https://github.com/graphitemaster/incbin

project "bin2c_incbin"
	kind "ConsoleApp"

	includedirs {
		bx_includedirs,
		fmtlib_includedirs,
	}

	files {
		"./bin2c_incbin/bin2c_incbin.cpp",
		"./bin2c_incbin/**.h",
	}

	links {
		"bx",
		"fmtlib",
	}

	configuration { "mingw-*" }
		targetextension ".exe"

	configuration {}

	strip()

---

project "bin2cppvec_incbin"
	kind "ConsoleApp"

	includedirs {
		bx_includedirs,
		fmtlib_includedirs,
	}

	files {
		"./bin2c_incbin/bin2cppvec_incbin.cpp",
		"./bin2c_incbin/**.h",
	}

	links {
		"bx",
		"fmtlib",
	}

	configuration { "mingw-*" }
		targetextension ".exe"

	configuration {}

	strip()

---

project "bin2cppstring_incbin"
	kind "ConsoleApp"

	includedirs {
		bx_includedirs,
		fmtlib_includedirs,
	}

	files {
		"./bin2c_incbin/bin2cppstring_incbin.cpp",
		"./bin2c_incbin/**.h",
	}

	links {
		"bx",
		"fmtlib",
	}

	configuration { "mingw-*" }
		targetextension ".exe"

	configuration {}

	strip()

---
