commander_native

C++ plugin for file commander

BUILD:
node-gyp configure
node-gyp build

TEST PUBLISH:
npm pack

PUBLISH:
npm login
npm publish

UPDATE:
npm login
npm version <t>
t: patch, minor, or major
npm publish

DEMO:

import * as native from "commander_native"

native.getFiles("c:\\", (_, result) => {
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