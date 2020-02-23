import { FileSystem } from 'react-native-unimodules';

export class DataLoader {
    static load(path, defaultContent, callback) {
        FileSystem.getInfoAsync(path).then((info) => {
            console.log(typeof path);
            if (!info.exists) {
                FileSystem.readAsStringAsync(path).then((content) => callback(true, content));
            } else {
                FileSystem.writeAsStringAsync(path, defaultContent, { encoding: FileSystem.EncodingType.UTF8 }).then(() => callback(false, defaultContent));
            }
        });
    }

    static loadInfo(callback) {
        DataLoader.load(global.infoPath, require("./info.json"), callback);
    }

    static loadTemplates(callback) {
        DataLoader.load(global.templatePath, require("./templates.json"), callback);
    }

    static loadNotes(callback) {
        DataLoader.load(global.notePath, require("./notes.json"), callback);
    }
}