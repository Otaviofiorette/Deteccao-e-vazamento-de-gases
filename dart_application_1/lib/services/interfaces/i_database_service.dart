import '../../models/leitura.dart';

abstract class IDatabaseService {
  Future<void> connect();
  Future<void> close();

  Future<void> upsertLeitura(Leitura l);
}