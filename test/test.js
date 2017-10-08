const fs = require('fs');
const getFiles  = require('../build/Release/commander_native').getFiles  
const getIcon  = require('../build/Release/commander_native').getIcon  

console.log("Start testing...")

try {
    getFiles('/', (error, result) => {
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
    })

    getIcon(".html", (error, result) => {
        console.log("Callback:")
        if (error) {
            console.log(error)
            return
        }
        //fs.writeFileSync("test.png", result);
    })
}
catch (err) {
    console.log(err)
}

console.log("End testing...")