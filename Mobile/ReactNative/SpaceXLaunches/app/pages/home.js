import React, { Component } from "react";
import { View, Text, Button, TextInput, TouchableOpacity, Image } from "react-native";
import { HeaderButton } from "./components";

// style
import * as styles from "../assets/style/style";

// data
import * as Data from "../data/data";

export class HomeScreen extends Component {
    constructor({ props }) {
        super(props);
        this.state = { name: "", ready: false };
    }

    componentDidMount() {
        if (!global.dataLoaded) {
            Data.fetchData().then(() => {
                this.setState({ ready: true });
            });
        } else {
            this.setState({ ready: true });
        }
    }

    search() {
        return global.launchNames.indexOf(this.state.name);
    }

    render() {
        const { navigation } = this.props;

        if (this.state.ready) {
            return (
                <View style={[styles.mainStyles.container]}>
                    <Button
                        title="Latest Launch"
                        onPress={() => {
                            var idx = global.pastLaunches.length - 1;
                            navigation.navigate("launch", {
                                listIdx: idx,
                                name: global.launchNames[idx]
                            });
                        }}
                    />
                    <View>
                        <TextInput
                            style={{ height: 40 }}
                            placeholder="Launch name"
                            onChangeText={(name) => this.setState({ name })}
                            value={this.state.name}
                        />
                        <Button
                            title="Search"
                            onPress={() => {
                                var idx = this.search();
                                console.log(idx);
                                if (idx == -1) {
                                    alert("Launch name not found");
                                } else {
                                    navigation.navigate("launch", {
                                        listIdx: idx,
                                        name: this.state.name
                                    });
                                }
                            }}
                        />
                    </View>
                    <Button
                        title="Random Launch"
                        onPress={() => {
                            var idx = Math.floor((global.launchNames.length - 1) * Math.random());
                            navigation.navigate("launch", {
                                listIdx: idx,
                                name: global.launchNames[idx]
                            });
                        }}
                    />
                    <View>
                        <Text>Stats:</Text>
                        <View style={[styles.mainStyles.row]}>
                            <TouchableOpacity
                                onPress={() => navigation.navigate("list", {
                                    lowerRange: 0,
                                    upperRange: global.pastLaunches.length,
                                    name: "Completed Launches"
                                })}
                                style={[styles.mainStyles.button]}
                            >
                                <Text style={[styles.mainStyles.textImportant]}>{global.pastLaunches.length.toString()}</Text>
                                <Text style={[styles.mainStyles.textCenter]}>completed launches</Text>
                            </TouchableOpacity>
                            <TouchableOpacity
                                onPress={() => navigation.navigate("list", {
                                    lowerRange: global.pastLaunches.length,
                                    upperRange: global.launchNames.length,
                                    name: "Planned Launches"
                                })}
                                style={[styles.mainStyles.button]}
                            >
                                <Text style={[styles.mainStyles.textImportant]}>{global.upcomingLaunches.length.toString()}</Text>
                                <Text style={[styles.mainStyles.textCenter]}>planned launches</Text>
                            </TouchableOpacity>
                        </View>
                    </View>

                    <HeaderButton navigation={navigation} name="about" />
                </View>
            );
        } else {
            return (
                <View>
                    <Text>Loading data</Text>

                    <HeaderButton navigation={navigation} name="about" />
                </View>
            );
        }
    }
}