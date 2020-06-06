import React, { Component } from "react";
import { View, Text, Button } from "react-native";

// navigation
import { NavigationContainer } from '@react-navigation/native';
import { createStackNavigator } from '@react-navigation/stack';

// style
import * as styles from "../style/style";

export function LaunchScreen({ route, navigation }) {
    const { listId } = route.params;

    return (
        <View styles={[styles.mainStyles]}>
            <Text>listId: {JSON.stringify(listId)}</Text>
        </View>
    );
}