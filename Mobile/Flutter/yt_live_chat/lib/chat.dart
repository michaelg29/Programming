import "dart:async";

import "package:flutter/material.dart";

import "utils/connection.dart" as cnxn;
import "utils/fstream.dart" as fstream;

import "assets/style/styles.dart" as styles;

class ChatMsg {
  final String authorChannelId;
  String author;
  final String publishDate;
  final String displayMessage;

  ChatMsg({this.authorChannelId, this.publishDate, this.displayMessage}) {}

  factory ChatMsg.fromJson(Map<String, dynamic> json) {
    return ChatMsg(
      authorChannelId: json["snippet"]["authorChannelId"],
      displayMessage: json["snippet"]["displayMessage"],
    );
  }
}

class ChatPage extends StatefulWidget {
  final Map<String, dynamic> args;

  ChatPage({Key key, @required this.args}) : super(key: key);

  @override
  ChatPageState createState() {
    return ChatPageState(
        vidId: args["vidId"],
        liveChatId: args["chatId"],
        apiKey: args["api_key"]);
  }
}

class ChatPageState extends State<ChatPage> {
  final String vidId;
  final String liveChatId;
  List<Map<String, String>> chat;

  String apiKey;

  Timer timer;

  ChatPageState(
      {@required this.vidId,
      @required this.liveChatId,
      @required this.apiKey}) {
    chat = [];

    print("start");
    Future.delayed(const Duration(seconds: 2), () {
      timer = Timer.periodic(Duration(seconds: 1), (timer) {
        print("execute");
        setState(() {});
      });
    });
  }

  @override
  void dispose() {
    super.dispose();
    timer.cancel();
  }

  ListView buildList(List<Map<String, String>> newChat) {
    if (newChat != null) {
      chat = newChat;
    }

    return ListView.builder(
      padding: const EdgeInsets.all(5.0),
      itemCount: chat.length,
      itemBuilder: (context, i) {
        return ListTile(
          title: Text(chat[i]["name"], style: styles.channelName),
          subtitle: Text(chat[i]["content"]),
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Live Chat"),
        actions: <Widget>[
          IconButton(
            icon: Icon(Icons.info),
            onPressed: () {
              return showDialog(
                context: context,
                builder: (context) {
                  String info = "ID: ${vidId}\n" + "Chat ID: ${liveChatId}\n";

                  if (chat != null) {
                    info += "Number of Messages: ${chat.length}\n";
                  }

                  return AlertDialog(
                    content: Text(info),
                  );
                },
              );
            },
          ),
        ],
      ),
      body: FutureBuilder<List<Map<String, String>>>(
          future: cnxn.genList(apiKey, liveChatId),
          builder: (context, snapshot) {
            if (snapshot.hasError) print(snapshot.error);

            return buildList(snapshot.hasData ? snapshot.data : null);
          }),
    );
  }
}
