const fs = require('fs');
const getFiles  = require('../build/Release/commander_native').getFiles  
const getIcon  = require('../build/Release/commander_native').getIcon  
const getExtendedInfos  = require('../build/Release/commander_native').getExtendedInfos  

console.log("Start testing...")

try {
    const path = 'c:\\windows\\system32\\'
    getFiles(path, (error, result) => {
        console.log("Callback:")
        if (error) {
            console.log(error)
            return
        }

        console.log(result.length)
        result.forEach(n => {
            console.log('')
            console.log('= NEW ======================')
            console.log(n.displayName)
            console.log(n.isDirectory)
            console.log(n.isHidden)
            console.log(n.size)
            console.log(n.time)
        })

        console.log("==============================================")
        var files = result.filter(n => n.displayName.endsWith(".exe") || n.displayName.endsWith(".dll")).map(n => n.displayName)
        console.log("==============================================")

        getExtendedInfos(path, files, (err, res) => {
            res.forEach(version => {
                console.log(version)
            })
        })
    })

    // getIcon(".html", (error, result) => {
    //     console.log("Callback:")
    //     if (error) {
    //         console.log(error)
    //         return
    //     }
    //     fs.writeFileSync("./test/test.png", result);
    // })
}
catch (err) {
    console.log(err)
}

// setInterval(() => {
//     console.log("Tick...")        
//     global.gc()
// }, 3000)

console.log("End testing...")