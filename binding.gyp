{
    "targets": [{
        "target_name": "commander_native",
        "sources": [ 
            "./commander-node.cpp", 
            "./worker.cpp", 
            "./windows/windows.cpp", 
            "./windows/iconextractor245.cpp", 
            "./windows/memorystream.cpp", 
            "./linux/linux.cpp" ],
        "include_dirs": [
            "<!(node -e \"require('nan')\")"
        ],
        "cflags": ["-Wall", "-std=c++14"],
        'link_settings': {
            "libraries": [ 
                "gdiplus.lib",
                "Minncore.lib"
            ]
        },
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
                    'libraries!': [ 
                        "gdiplus.lib",
                        "Mincore.lib"
                    ]
                }
            ],
        ]  
    }]
}