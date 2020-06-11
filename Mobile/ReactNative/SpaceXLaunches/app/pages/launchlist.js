import React, { Component, PureComponent } from "react";
import { View, Text, FlatList, TouchableOpacity, Image } from "react-native";
import { HeaderButton } from "./components";

// style
import * as styles from "../assets/style/style";

class LaunchListItem extends PureComponent {
    render() {
        const { name, navigation } = this.props;
        const { iconUri, date, listIdx } = this.props.nameData;

        return (
            <TouchableOpacity
                style={[styles.mainStyles.listItem]}
                onPress={() => {
                    navigation.navigate("launch", {
                        listIdx: listIdx,
                        name: name
                    });
                }}
            >
                <Image
                    style={[styles.mainStyles.imageIcon, styles.mainStyles.listItemIcon]}
                    source={{ uri: iconUri }}
                />
                <View style={[styles.mainStyles.listItemText]}>
                    <Text style={[{ fontWeight: "bold" }]}>{name}</Text>
                    <Text>{date.toLocaleDateString()}</Text>
                </View>
            </TouchableOpacity>
        );
    }
}

export class LaunchListScreen extends Component {
    render() {
        const { navigation, route } = this.props;

        const { lowerRange, upperRange } = route.params;
        const nameRange = global.launchNames.slice(lowerRange, upperRange);
        const listData = nameRange.map((name) => { return { key: name }; });

        return (
            <View style={[styles.mainStyles.container]}>
                <FlatList
                    style={[styles.mainStyles.paddedList]}
                    data={listData}
                    initialNumToRender={8}
                    renderItem={({ item }) => <LaunchListItem
                        name={item.key}
                        nameData={global.nameData[item.key]}
                        navigation={navigation} />}
                />

                <HeaderButton navigation={navigation} name="about" />
            </View>
        );
    }
}