import React, { Component } from "react";
import { View, Text, ScrollView, FlatList } from "react-native";
import { HeaderButton, Link, FullWidthImage } from "./components";

// style
import * as styles from "../assets/style/style";

export class LaunchScreen extends Component {
    render() {
        const { navigation, route } = this.props;
        const { listIdx, name } = route.params;

        let item;

        if (global.nameData[name].upcoming) {
            item = global.upcomingLaunches[listIdx - global.pastLaunches.length];
        } else {
            item = global.pastLaunches[listIdx];
        }

        let listData = item["links"]["flickr_images"].map((link) => { return { key: link } });

        return (
            <View styles={[styles.mainStyles.container]}>
                <ScrollView>
                    <Text style={[styles.mainStyles.launchItem]}>Flight number {item["flight_number"]}</Text>
                    <Text style={[styles.mainStyles.launchItem]}>Launch date/time: {new Date(item["launch_date_local"]).toLocaleString()}</Text>
                    <Text style={[styles.mainStyles.launchItem]}>This flight was a {item["launch_success"] ? "success" : "failure"}</Text>
                    <Text style={[styles.mainStyles.launchItem]}>Launch details: {item["details"]}</Text>

                    <Text style={[styles.mainStyles.header]}>Media:</Text>

                    <Link text="Launch Video" url={item["links"]["video_link"]} />
                    <Link text="Article" url={item["links"]["article_link"]} />
                    <Link text="Presskit" url={item["links"]["presskit"]} />
                    <Link text="Reddit Campaign Thread" url={item["links"]["reddit_campaign"]} />
                    <Link text="Reddit Launch Thread" url={item["links"]["reddit_launch"]} />

                    <Text style={[styles.mainStyles.header]}>Images:</Text>

                    <Link text="Reddit Media" url={item["links"]["reddit_media"]} />

                    <FullWidthImage uri={item["links"]["mission_patch"]} />

                    <FlatList
                        data={listData}
                        initialNumToRender={1}
                        renderItem={({ item }) => <FullWidthImage uri={item.key} />}
                    />
                </ScrollView>

                <HeaderButton navigation={navigation} name="about" />
            </View>
        );
    }
}