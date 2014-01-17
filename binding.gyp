{
    "targets": [
	{
	    "target_name": "iwlib",
	    "sources": [ 
		"src/module.cpp",
		"src/node-iwlib.cpp",
	    ],
	    "cflags_cc" : [ "-fexceptions" ],
	    "link_settings" : {
		"libraries" : [ "-liw"]
	    }
	}
    ],
}
