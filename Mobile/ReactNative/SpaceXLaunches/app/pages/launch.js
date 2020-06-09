import React, { Component } from "react";
import { View, Text, Button } from "react-native";
import { HeaderButton } from "./components";

// style
import * as styles from "../assets/style/style";

export class LaunchScreen extends Component {
    render() {
        const { navigation, route } = this.props;
        const { listIdx } = route.params;

        return (
            <View styles={[styles.mainStyles.container]}>
                <Text>listId: {JSON.stringify(listIdx)}</Text>

                <HeaderButton navigation={navigation} name="about" />
            </View>
        );
    }
}