interface FileItem {
    displayName: string
    size: number
    time: Date
    isDirectory: boolean
    isHidden: boolean
}

declare module 'commander_native' {
    function getFiles(path: string, callback: (error: any, result: FileItem[]) => void): void
    function getIcon(extension: string, callback: (error: any, result: Buffer) => void): void
    function getExtendedInfos(path: string, files: string[], callback: (err, res: string[]) => void): void
}