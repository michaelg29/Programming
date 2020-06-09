import React, { Component } from "react";
import { TouchableOpacity, Image } from "react-native";

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