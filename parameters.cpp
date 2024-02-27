#pragma once
const int bandwidth = 1667;
const int newMessageLimit = 1667;
const int gossipRate = 32;
const int numOfNodes = 10000;
const int numOfThreads = 128;
const int totalRounds = 200;
const int usefulRound = 60;

const int numOfMessagesTotal = 1667 * 60;

// bandiwth: 200Mbps new message limit: 200Mbps gossip rate: 32

// for attack
const int numOfDeadNodes = 0;

const int logFrequency = 1;
const int messageRecordFrequency = 60;

// for random
int b, t;