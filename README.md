commander-windows

C++ plugin for file commander

BUILD:
node-gyp configure
node-gyp build

PUBLISH:
npm login
npm publish

UPDATE:

DEMO:

import * as commanderWindows from "commander_windows"

commanderWindows.getFiles("c:\\", (_, result) => {
    const items = result.map(n => {
        return {
            displayName: n.displayName,
            isHidden: n.isHidden,
            size: n.size,
            date: n.time,
            isDirectory: n.isDirectory                                              
        }
    })
    console.log(items)
})