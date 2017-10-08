
const getFiles  = require('../build/Release/commander_native').getFiles  

console.log("Start testing...")

try {
    getFiles('/', (error, result) => {

        if (error) {
            console.log(error)
            return
        }

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
}
catch (err) {
    console.log(err)
}

console.log("End testing...")