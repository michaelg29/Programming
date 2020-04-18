import 'package:path_provider/path_provider.dart';
import "dart:io";

Future<String> get _localPath async {
  final directory = await getApplicationDocumentsDirectory();

  return directory.path;
}

Future<File> get _localFile async {
  final path = await _localPath;
  return File("$path/apikey.txt");
}

Future<String> getApiKey() async {
  try {
    final file = await _localFile;

    String contents = await file.readAsString();

    return contents;
  } catch (e) {
    return null;
  }
}

Future<File> writeApiKey(String api_key) async {
  final file = await _localFile;
  return file.writeAsString("$api_key");
}
