import "dart:async";
import 'dart:convert';

import "package:http/http.dart" as http;

/*
Channel Info
https://www.googleapis.com/youtube/v3/channels?part=snippet&id=<CHANNEL_ID>&key=<API_KEY>

Get Live Chat ID
res = GET https://www.googleapis.com/youtube/v3/videos?key=<API_KEY>&id=<VIDEO_ID>&part=snippet,contentDetails,statistics,liveStreamingDetails
LIVE_CHAT_ID = res.items[0].liveStreamingDetails.activeLiveChatId

https://www.googleapis.com/youtube/v3/liveChat/messages?key=<API_KEY>&liveChatId=<LIVE_CHAT_ID>&part=snippet
*/

Future<String> getLiveChatId(String api_key, String videoId) async {
  final String url =
      "https://www.googleapis.com/youtube/v3/videos?key=${api_key}&id=${videoId}&part=snippet,contentDetails,statistics,liveStreamingDetails";
  final response = await http.Client().get(url);

  final items = jsonDecode(response.body)["items"];
  if (items.length == 0) {
    // video not found
    return "";
  } else {
    return items[0]["liveStreamingDetails"]["activeLiveChatId"].toString();
  }
}

Future<List> getChat(String api_key, String liveChatId) async {
  final String url =
      "https://www.googleapis.com/youtube/v3/liveChat/messages?key=${api_key}&liveChatId=${liveChatId}&part=snippet";
  final response = await http.Client().get(url);

  return jsonDecode(response.body)["items"];
}

Future<String> getChannelName(String api_key, String channelId) async {
  final String url =
      "https://www.googleapis.com/youtube/v3/channels?key=${api_key}&part=snippet&id=${channelId}";
  final response = await http.Client().get(url);

  return jsonDecode(response.body)["items"][0]["snippet"]["title"];
}

Future<List<Map<String, String>>> genList(
    String api_key, String liveChatId) async {
  final entireChat = await getChat(api_key, liveChatId);
  List<Map<String, String>> parsedChat = [];

  for (int i = 0; i < entireChat.length; i++) {
    String channelId = entireChat[i]["snippet"]["authorChannelId"];
    String content = entireChat[i]["snippet"]["displayMessage"];
    String msgId = entireChat[i]["id"];

    parsedChat.add({
      "name": await getChannelName(api_key, channelId),
      "content": content,
      "id": msgId,
    });
  }

  return parsedChat;
}
