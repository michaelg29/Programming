import React, { Component } from 'react';
import {
    SafeAreaView,
    ScrollView,
} from 'react-native';

import { Header, Text, Button, Input, Icon } from 'react-native-elements';

import { styles } from './styles';

export class Settings extends Component {
    constructor(props) {
        super(props);

        this.state = { name: global.info.name };
    }

    submit() {
        const { name } = this.state;

        this.props.navigation.navigate("dashboard");
    }

    render() {
        return (
            <SafeAreaView>
                <ScrollView>
                    <Header statusBarProps={{ barStyle: 'light-content' }}
                        barStyle="light-content"
                        containerStyle={{
                            backgroundColor: '#3D6DCC',
                            justifyContent: 'space-around',
                        }}>
                        <Button
                            icon={
                                <Icon
                                    name="home"
                                    size={15}
                                    color="white"
                                />
                            }
                            iconLeft
                            type="clear"
                            onPress={this.submit.bind(this)}
                        />
                        <Text style={{ color: "#fff" }}>SETTINGS</Text>
                    </Header>
                    <Input
                        placeholder="Name"
                        leftIcon={
                            <Icon
                                name="person"
                                size={24}
                                color="black"
                            />
                        }
                        label="Name"
                        onChangeText={(text) => this.setState({ name: text })}
                        value={this.state.text}
                    />
                    <Button
                        title="Save"
                        type="solid"
                        onPress={this.submit.bind(this)}
                    />
                </ScrollView>
            </SafeAreaView>
        );
    }
}