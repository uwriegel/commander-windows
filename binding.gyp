{
    "targets": [{
        "target_name": "commander_native",
        "sources": [ 
            "./commander-node.cpp", 
            "./windows/windows.cpp", 
            "./windows/iconextractor.cpp", 
            "./windows/memorystream.cpp", 
            "./linux/linux.cpp" ],
        "cflags": ["-Wall", "-std=c++14"],
        "libraries": [ "gdiplus.lib" ],
        "conditions": [
            ['OS=="win"', {
                    'sources!': ['./linux/linux.cpp']
                }
            ],
            ['OS=="linux"', {
                    'sources!': [
                        './windows/windows.cpp', 
                        './windows/iconextractor.cpp',
                        "./windows/memorystream.cpp"
                    ],
                    'libraries!': [ "gdiplus.lib" ]
                }
            ],
        ]  
    }]
}