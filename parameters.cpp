#pragma once
const int bandwidth = 17;
const int newMessageLimit = 34;
const int gossipRate = 32;
const int numOfNodes = 100;
const int numOfThreads = 4;
const int totalRounds = 20000;
const int usefulRound = 10000;

const int numOfMessagesTotal = 17 * 10000;

// bandwidth: 200Mbps new message limit: 200Mbps gossip rate: 32

// for attack
const int numOfDeadNodes = 0;

const int logFrequency = 1;
const int messageRecordFrequency = 60;

const int prevMessagesRounds = 3;
// for random
int b, t;