--
-- Copyright 2010-2017 Branimir Karadzic. All rights reserved.
-- License: https://github.com/bkaradzic/bx#license-bsd-2-clause
--

project "bin2c"
	kind "ConsoleApp"

	includedirs {
		path.join(BX_DIR, "include"),
	}

	files {
		"./bin2c/**.cpp",
		"./bin2c/**.h",
	}

	links {
		"bx",
	}

	configuration { "mingw-*" }
		targetextension ".exe"

	configuration {}

	strip()

---

project "bin2cpparr"
	kind "ConsoleApp"

	includedirs {
		path.join(BX_DIR, "include"),
	}

	files {
		"./bin2cpparr/**.cpp",
		"./bin2cpparr/**.h",
	}

	links {
		"bx",
	}

	configuration { "mingw-*" }
		targetextension ".exe"

	configuration {}

	strip()

---

project "bin2cppvec"
	kind "ConsoleApp"

	includedirs {
		path.join(BX_DIR, "include"),
	}

	files {
		"./bin2cppvec/**.cpp",
		"./bin2cppvec/**.h",
	}

	links {
		"bx",
	}

	configuration { "mingw-*" }
		targetextension ".exe"

	configuration {}

	strip()

---

project "bin2cppstring"
	kind "ConsoleApp"

	includedirs {
		path.join(BX_DIR, "include"),
	}

	files {
		"./bin2cppstring/**.cpp",
		"./bin2cppstring/**.h",
	}

	links {
		"bx",
	}

	configuration { "mingw-*" }
		targetextension ".exe"

	configuration {}

	strip()
