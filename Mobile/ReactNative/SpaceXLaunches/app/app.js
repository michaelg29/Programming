import React from "react";

// navigation
import { NavigationContainer } from "@react-navigation/native";
import { createStackNavigator } from "@react-navigation/stack";

// pages
import * as HomePage from "./pages/home";
import * as AboutPage from "./pages/about";
import * as LaunchPage from "./pages/launch";
import * as LaunchListPage from "./pages/launchlist";

const Stack = createStackNavigator();
export default function App() {
    // initialize data variables
    global.dataLoaded = false;
    global.upcomingLaunches = [];
    global.pastLaunches = [];
    global.launchNames = [];
    global.nameData = {};
    global.dateLength = "YYYY-MM-DDTHH:MM:SS".length;

    return (
        <NavigationContainer>
            <Stack.Navigator
                initialRouteName="home"
                screenOptions={{
                    headerStyle: {
                        backgroundColor: "#f4511e",
                        height: 56
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
                        //headerShown: false
                        headerStyle: {
                            backgroundColor: "#000846"
                        },
                        title: "SpaceX Launches"
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
                <Stack.Screen
                    name="list"
                    component={LaunchListPage.LaunchListScreen}
                    options={({ route }) => ({ title: route.params.name })}
                />
            </Stack.Navigator>
        </NavigationContainer>
    );
}