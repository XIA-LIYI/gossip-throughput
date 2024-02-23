metric
through: average throughput
latency: average round that one message takes to reach to 95% nodes
failure: percentage of messages that do not reach to all of nodes

setting: 20,000 nodes, 200Mbps, 16 receivers per message
| metric | with attack (20% nodes are dead) | without attack|
|:---:|:---:|:---:|
| throughput |~30%|~53%|
|latency|7|6|
|failure|20%|0.22%|

setting: 20,000 nodes, 200Mbps, 32 receivers per message
| metric | with attack (20% nodes are dead) | without attack|
|:---:|:---:|:---:|
| throughput |~45%|~60%|
|latency|6|6|
|failure|14%|0%|

setting: 20,000 nodes, 200Mbps, 64 receivers per message
| metric | with attack (20% nodes are dead) | without attack|
|:---:|:---:|:---:|
| throughput |~45%|~60%|
|latency|6|6|
|failure|14%|0%|

setting: 30,000 nodes, 200Mbps, 16 receivers per message
| metric | with attack (20% nodes are dead) | without attack|
|:---:|:---:|:---:|
| throughput ||~57%|
|latency||7|
|failure||0.3%|

setting: 30,000 nodes, 200Mbps, 32 receivers per message
| metric | with attack (20% nodes are dead) | without attack|
|:---:|:---:|:---:|
| throughput ||~57%|
|latency||7|
|failure||0.3%|

191965
191964
