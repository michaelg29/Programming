import React, { Component } from 'react';
import {
    SafeAreaView,
    ScrollView,
    Text,
    View,
} from 'react-native';

import { DataLoader } from '../data/load';

import { Dashboard } from './dashboard';
import { Settings } from './settings';

import { FileSystem, Permissions } from 'react-native-unimodules';

export class Primary extends Component {
    constructor(props) {
        console.log("=======================================================\n=====================================================");

        super(props);
        this.state = { ready: false, create: false };

        global.dataPath = `${FileSystem.documentDirectory}`;
        global.infoPath = `${global.dataPath}/info.json`;
        global.templatePath = `${global.dataPath}/templates.json`;
        global.notePath = `${global.dataPath}/notes.json`;
    }

    async componentDidMount() {
        if (!this.state.ready) {
            DataLoader.loadInfo((fileExists, content) => {
                global.info = content;
                this.setState({ ready: true, create: !fileExists });
            });

            DataLoader.loadTemplates((fileExists, content) => {
                global.templates = content;
            });

            DataLoader.loadNotes((fileExists, content) => {
                global.notes = content;
            });
        }
    }

    render() {
        if (this.state.ready) {
            console.log(global.info);
            if (this.state.create) {
                // set settings
                return <Settings navigation={this.props.navigation} />
            } else {
                // goto dashboard
                return <Dashboard navigation={this.props.navigation} />
            }
        } else {
            // TODO: Add splash screen
            return (
                <View>
                    <Text>Loading</Text>
                </View>
            );
        }
    }
}