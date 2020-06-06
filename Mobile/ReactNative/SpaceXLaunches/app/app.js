import React, { Component } from "react";
import { View, Text } from "react-native";

// navigation
//import { NavigationContainer } from "@react-navigation/native";
import { NavigationContainer } from "@react-navigation/native";
import { createStackNavigator } from "@react-navigation/stack";

// pages
import * as HomePage from "./pages/home";
import * as AboutPage from "./pages/about";
import * as LaunchPage from "./pages/launch";

const Stack = createStackNavigator();
export default function App() {
    return (
        <NavigationContainer>
            <Stack.Navigator
                initialRouteName="home"
                screenOptions={{
                    headerStyle: {
                        backgroundColor: "#f4511e",
                    },
                    headerTintColor: "#fff",
                    headerTitleStyle: {
                        fontWeight: "bold"
                    }
                }}>
                <Stack.Screen
                    name="home"
                    component={HomePage.HomeScreen}
                    options={{
                        headerShown: false
                    }}
                />
                <Stack.Screen
                    name="about"
                    component={AboutPage.AboutScreen}
                    options={{ title: "About" }}
                />
                <Stack.Screen
                    name="launch"
                    component={LaunchPage.LaunchScreen}
                    options={({ route }) => ({ title: route.params.name })}
                />
            </Stack.Navigator>
        </NavigationContainer >
    );
}