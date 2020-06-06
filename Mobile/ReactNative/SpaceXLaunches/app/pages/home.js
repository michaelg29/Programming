import React, { Component } from "react";
import { View, Text, Button } from "react-native";

// navigation
import { NavigationContainer } from '@react-navigation/native';
import { createStackNavigator } from '@react-navigation/stack';

// style
import * as styles from "../style/style";

export function HomeScreen({ navigation }) {
    return (
        <View style={[styles.mainStyles]}>
            <Text>Hello, world!</Text>
            <Button
                title="About this App"
                onPress={() => navigation.navigate("about")}
            />
            <Button
                title="Search"
                onPress={() => navigation.navigate("launch", {
                    listId: 312,
                    name: "Mission"
                })}
            />
        </View>
    );
}