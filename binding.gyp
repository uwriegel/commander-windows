{
    "targets": [{
        "target_name": "commander_native",
        "sources": [ "./commander-node.cpp", "./windows.cpp", "./linux.cpp" ],
        "conditions": [
            ['OS=="win"', {'sources!': ['./linux.cpp']}],
            ['OS=="linux"', {'sources!': ['./windows.cpp']}],
        ],  
        "cflags": ["-Wall", "-std=c++14"]
    }]
}