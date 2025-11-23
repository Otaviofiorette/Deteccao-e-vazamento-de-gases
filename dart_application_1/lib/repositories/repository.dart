import '../services/firebase/firebase_service.dart';
import '../services/interfaces/i_database_service.dart';
import '../models/leitura.dart';
import '../utils/logger.dart';

class Repository {
  final FirebaseService firebase;
  final IDatabaseService db;

  Repository({required this.firebase, required this.db});

  Future<void> syncOnce() async {
    final leiturasMap = await firebase.getNode('leituras') ?? {};

    for (final entry in leiturasMap.entries) {
      final leituraData = entry.value;

      if (leituraData is Map<String, dynamic>) {
        try {
          final leitura = Leitura.fromMapSafe(leituraData);
          if (leitura == null) {
            Logger.error('Leitura inválida (campos obrigatórios ausentes) chave=${entry.key}');
            continue;
          }

          await db.upsertLeitura(leitura);
        } catch (err) {
          Logger.error('Erro processando leitura ${entry.key}: $err');
        }
      }
    }

    Logger.info('Sync concluído');
  }
}