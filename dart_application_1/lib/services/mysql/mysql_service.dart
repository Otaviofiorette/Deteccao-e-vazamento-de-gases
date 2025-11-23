import 'package:mysql1/mysql1.dart';
import '../../services/interfaces/i_database_service.dart';
import '../../models/leitura.dart';

class MySQLService implements IDatabaseService {
  final ConnectionSettings settings;
  MySqlConnection? _conn;

  MySQLService(this.settings);

  @override
  Future<void> connect() async {
    _conn = await MySqlConnection.connect(settings);
  }

  @override
  Future<void> close() async {
    await _conn?.close();
    _conn = null;
  }

  MySqlConnection get _c {
    if (_conn == null) throw StateError('MySQL não conectado');
    return _conn!;
  }

  @override
  Future<void> upsertLeitura(Leitura l) async {
    final dataHoraStr = l.dataHora == null
        ? null
        : l.dataHora!.toIso8601String().split('.').first;

    await _c.query(
      'INSERT INTO leituras (idLeitura, dataHora, valor, alerta, sensor_id) '
      'VALUES (?, ?, ?, ?, ?) '
      'ON DUPLICATE KEY UPDATE dataHora=VALUES(dataHora), valor=VALUES(valor), alerta=VALUES(alerta), '
      'sensor_id=VALUES(sensor_id)',
      [
        l.idLeitura,
        dataHoraStr,
        l.valor,
        l.alerta ? 1 : 0,
        l.sensorId,
      ],
    );
  }
}