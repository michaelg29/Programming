import React, { Component } from "react";
import { TouchableOpacity, Image, Text, Linking, Dimensions, View } from "react-native";

import * as styles from "../assets/style/style";

const images = {
    "home": require("../assets/cgi-bin/home.png"),
    "about": require("../assets/cgi-bin/about.png")
}

export class HeaderButton extends Component {
    render() {
        const { navigation, name } = this.props;

        return (
            <TouchableOpacity
                onPress={() => navigation.navigate(name)}
                style={[styles.mainStyles.headerRightBtn]}
            >
                <Image
                    style={[styles.mainStyles.imageIcon]}
                    source={images[name]}
                />
            </TouchableOpacity>
        );
    }
}

export class Link extends Component {
    render() {
        const { text, url } = this.props;
        if (url === null) {
            return null;
        }

        return (
            <Text style={[styles.mainStyles.launchSubItem, styles.mainStyles.link]}
                onPress={() => Linking.openURL(url)}>
                {text}
            </Text>
        );
    }
}

export class FullWidthImage extends Component {
    render() {
        const { uri } = this.props;

        if (uri === null) {
            return null;
        }

        const width = Dimensions.get("window").width;

        return (
            <TouchableOpacity
                onPress={() => Linking.openURL(uri)}>
                <Image
                    source={{ uri: uri }}
                    style={[{ width: width, height: width, resizeMode: "contain" }]}
                />
            </TouchableOpacity>
        );
    }
}