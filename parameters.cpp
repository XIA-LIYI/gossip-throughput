#pragma once
const int bandwidth = 1667;
const int newMessageLimit = 1667;
const int gossipRate = 16;
const int numOfNodes = 10000;
const int numOfThreads = 20;
const int totalRounds = 100;

const int numOfMessagesTotal = 1667 * 100;

// bandiwth: 200Mbps new message limit: 200Mbps gossip rate: 32

// for attack
const int numOfDeadNodes = 2000;

const int logFrequency = 1;
const int messageRecordFrequency = 40;

// for random
int b, t;