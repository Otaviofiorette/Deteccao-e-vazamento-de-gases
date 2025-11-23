# Projeto Integrado - Detecção e vazamento de Gases

Este projeto tem como objetivo desenvolver um protótipo para detecção de vazamento de gases em ambientes industriais. O sistema utiliza a placa ESP32 conectada a sensores MQ2 e MQ135 para monitorar a presença de gases. As leituras são enviadas para o Firebase Realtime Database e, por meio de uma integração escrita em Dart, sincronizadas com um banco de dados MySQL. Os dados armazenados são posteriormente analisados e apresentados em dashboards desenvolvidos no Power BI.

## Arquitetura

Fluxo de dados:
- ESP32: coleta dos dados dos sensores e envia para o Firebase.
- Dart: sincronização dos dados do Firebase com o MySQL.
- MySQL: armazenamento estruturado das leituras e informações adicionais.
- Power BI: visualização dos indicadores e relatórios.

## Tecnologias Utilizadas

- C++
- Firebase Realtime Database
- Dart
- MySQL
- Power BI

## Indicadores do Dashboard

- Total de Leituras
- Total de Alertas
- Taxa de Alertas (%)
- Valor Médio das Leituras
- Leituras e Alertas por mês
- Alertas por máquina
- Gestão de usuários e cargos

## Equipe

- Kevin Henrique Benedito  
- Luis Felipe Alves do Nascimento  
- Otávio Fiorette  
- Vitor Leôncio Bartalini  
- Vitória Martins  
