import '../utils/converters.dart';

class Leitura {
  final int idLeitura;
  final DateTime? dataHora;
  final double? valor;
  final bool alerta;
  final int? sensorId;

  Leitura({
    required this.idLeitura,
    this.dataHora,
    this.valor,
    this.alerta = false,
    this.sensorId,
  });

  static Leitura? fromMapSafe(Map<String, dynamic> m) {
    final id = tryParseInt(m['idLeitura']);
    if (id == null) return null; // id obrigatório

    final data = tryParseDateTime(m['dataHora']);
    final val = tryParseDouble(m['valor']);
    final sensor = tryParseInt(m['sensor_id']);

    bool alertaBool = false;
    final a = m['alerta'];
    if (a != null) {
      final sa = a.toString().trim().toLowerCase();
      alertaBool = (sa == '1' || sa == 'true' || sa == 'yes');
    }

    return Leitura(
      idLeitura: id,
      dataHora: data,
      valor: val,
      alerta: alertaBool,
      sensorId: sensor,
    );
  }
}