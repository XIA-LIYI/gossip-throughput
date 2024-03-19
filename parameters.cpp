#pragma once
const int bandwidth = 17;
const int newMessageLimit = 34;
const int gossipRate = 32;
const int numOfNodes = 100;
const int numOfThreads = 4;
const int totalRounds = 20000;
const int usefulRound = 10000;

const int numOfMessagesTotal = 1667 * 10000;

// bandwidth: 200Mbps new message limit: 200Mbps gossip rate: 32

// for attack
const int numOfDeadNodes = 0;

const int logFrequency = 1;

const int numOfMessageRecord = 1667 * 1000;
const int messageRecordGap = 10;
const int messageRecordFrequency = 20;

const int prevMessagesRounds = 0;
// for random
int b, t;