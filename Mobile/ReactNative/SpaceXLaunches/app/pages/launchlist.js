import React, { Component } from "react";
import { View, Text, Button } from "react-native";
import { HeaderButton } from "./components";

// style
import * as styles from "../assets/style/style";

export class LaunchListScreen extends Component {
    render() {
        const { navigation, route } = this.props;

        const { lowerRange, upperRange } = route.params;

        return (
            <View style={[styles.mainStyles.container]}>
                <Text>Launch List</Text>
                <Text>lower: {JSON.stringify(lowerRange)}</Text>
                <Text>upper: {JSON.stringify(upperRange)}</Text>
                <HeaderButton navigation={navigation} name="about" />
            </View>
        );
    }
}