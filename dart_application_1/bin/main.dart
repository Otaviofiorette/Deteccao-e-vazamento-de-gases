import 'dart:async';
import 'package:mysql1/mysql1.dart';

import '../lib/config/env.dart' as env;
import '../lib/services/mysql/mysql_service.dart';
import '../lib/services/firebase/firebase_service.dart';
import '../lib/repositories/repository.dart';
import '../lib/utils/logger.dart';

Future<void> main() async {
  final fb = FirebaseService(
    baseUrl: env.Env.firebaseBaseUrl,
    authToken: env.Env.firebaseAuthToken,
  );

  final mysqlSettings = ConnectionSettings(
    host: env.Env.mysqlHost,
    port: env.Env.mysqlPort,
    user: env.Env.mysqlUser,
    password: env.Env.mysqlPassword,
    db: env.Env.mysqlDatabase,
  );

  final mysqlService = MySQLService(mysqlSettings);
  await mysqlService.connect();

  final repo = Repository(firebase: fb, db: mysqlService);

  Logger.info('Iniciando sincronizador');

  Timer.periodic(Duration(seconds: env.Env.pollingSeconds), (t) async {
    try {
      await repo.syncOnce();
    } catch (e) {
      Logger.error('Erro no sync: $e');
    }
  });
}