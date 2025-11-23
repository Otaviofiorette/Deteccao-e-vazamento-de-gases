import 'dart:convert';
import 'package:http/http.dart' as http;

class FirebaseService {
  final String baseUrl;
  final String? authToken;

  FirebaseService({required this.baseUrl, this.authToken});

  Uri _uri(String path) {
    final tokenPart = (authToken != null && authToken!.isNotEmpty) ? '?auth=$authToken' : '';
    return Uri.parse('$baseUrl/$path.json$tokenPart');
  }

  Future<Map<String, dynamic>?> getNode(String path) async {
    final response = await http.get(_uri(path));
    if (response.statusCode == 200) {
      final decoded = json.decode(response.body);
      if (decoded is Map<String, dynamic>) return decoded;
      return decoded == null ? null : {'value': decoded};
    } else {
      throw Exception('Firebase GET falhou: ${response.statusCode}');
    }
  }
}