interface FileItem {
    displayName: string
    size: number
    time: Date
    isDirectory: boolean
    isHidden: boolean
}

declare module 'commander_native' {
    function getFiles(path: string, callback: (error: any, result: FileItem[]) => void): void
}