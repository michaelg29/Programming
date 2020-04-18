import "package:flutter/material.dart";

import "utils/navigator.dart" as nav;
import "utils/connection.dart" as cnxn;
import "utils/fstream.dart" as fstream;

class EnterPage extends StatefulWidget {
  @override
  EnterPageState createState() {
    return EnterPageState();
  }
}

class EnterPageState extends State<EnterPage> {
  final _formKey = GlobalKey<FormState>();

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Stream ID Entry"),
      ),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Center(
          child: Form(
            key: _formKey,
            child: Column(
              children: <Widget>[
                TextFormField(
                  autofocus: true,
                  autocorrect: false,
                  initialValue: "E1HSOMDyysg",
                  decoration: InputDecoration(
                    labelText: "Input the video ID for the stream:",
                    hintText: "Video ID",
                  ),
                  validator: (value) {
                    if (value.isEmpty) {
                      return "Please input an ID.";
                    }

                    fstream.getApiKey().then((String key) {
                      final String apiKey = key;
                      cnxn.getLiveChatId(apiKey, value).then((String val) {
                        nav.sendTo(context, "/chat", args: {
                          "vidId": value,
                          "chatId": val,
                          "api_key": key
                        });
                      });
                    });

                    return null;
                  },
                ),
                Builder(
                  builder: (context) => RaisedButton(
                    child: Text("View Chat"),
                    onPressed: () {
                      _formKey.currentState.validate();
                    },
                  ),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}
