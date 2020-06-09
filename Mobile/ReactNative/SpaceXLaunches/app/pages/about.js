import React, { Component } from "react";
import { View, Text, Button } from "react-native";
import { HeaderButton } from "./components";

// data
import * as Data from "../data/data";

// style
import * as styles from "../assets/style/style";

export class AboutScreen extends Component {
    render() {
        const { navigation } = this.props;

        return (
            <View style={[styles.mainStyles.container]}>
                <Button
                    title="Refresh Data"
                    onPress={() => {
                        Data.fetchData().then(() => {
                            alert("Data refreshed");
                        });
                    }}
                />

                <HeaderButton navigation={navigation} name="home" />
            </View>
        );
    }
}