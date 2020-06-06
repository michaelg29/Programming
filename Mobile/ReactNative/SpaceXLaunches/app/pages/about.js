import React, { Component } from "react";
import { View, Text, Button } from "react-native";

import * as styles from "./../style/style";

export function AboutScreen({ navigation }) {
    return (
        <View style={[styles.mainStyles]}>
            <Text>About Page</Text>
            <Button
                title="Go back home"
                onPress={() => navigation.navigate("home")}
            />
        </View>
    );
}