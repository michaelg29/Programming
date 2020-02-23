import React, { Component } from 'react';
import {
    SafeAreaView,
    ScrollView,
    Text,
    View,
} from 'react-native';
import { Button, Icon } from "react-native-elements";

import { styles } from './styles';

export class Dashboard extends Component {
    gotoSettings() {
        this.props.navigation.navigate("settings");
    }

    render() {
        return (
            <SafeAreaView>
                <ScrollView>
                    <Button
                        icon={
                            <Icon
                                name="settings"
                                size={15}
                                color="white"
                            />
                        }
                        iconLeft
                        onPress={this.gotoSettings.bind(this)}
                    />
                </ScrollView>
            </SafeAreaView>
        );
    }
}