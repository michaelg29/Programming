import { StyleSheet } from "react-native";

export const mainStyles = StyleSheet.create({
    container: {
        flex: 1,
        flexDirection: "column",
        justifyContent: "space-around"
    },
    imageIcon: {
        margin: 5,
        height: 46,
        width: 46,
        resizeMode: 'stretch'
    },
    row: {
        flexDirection: "row",
        justifyContent: "space-around"
    },
    button: {
        width: "50%",
        height: 46,
        margin: 4,
        flex: 1,
        flexDirection: "column",
        justifyContent: "center"
    },
    headerRightBtn: {
        position: "absolute",
        top: -56,
        right: 0,
        height: 56,
        width: 56,
        zIndex: 5
    },
    textCenter: {
        textAlign: "center"
    },
    textImportant: {
        fontWeight: "bold",
        fontSize: 36,
        textAlign: "center"
    },
    listItem: {
        width: "100%",
        height: 56,
        flexDirection: "row",
        padding: 5,
        margin: 5
    },
    listItemIcon: {
        width: 46,
        height: 46,
        margin: 5,
        flexDirection: "column",
        justifyContent: "center"
    },
    listItemText: {
        height: 46,
        flexDirection: "column",
        justifyContent: "center",
        marginLeft: 15
    },
    paddedList: {
        marginLeft: 15
    },
    launchItem: {
        marginHorizontal: 15,
        marginVertical: 5
    },
    header: {
        fontWeight: "bold",
        fontSize: 36,
        marginLeft: 10
    },
    launchSubItem: {
        marginLeft: 10
    },
    link: {
        color: "blue"
    }
});