source :: {FromDump(source.pcap, TIMING true, STOP true) -> VLANDecap -> output};
// source :: {FromDevice(eno1, PROMISC true) -> VLANDecap -> output};
sink :: {input -> IPEncap(4, 1.1.1.1, 2.2.2.2) -> EtherEncap(0x0800, 1:1:1:1:1:1, 2:2:2:2:2:2) -> ToDump(sink.pcap, SNAPLEN 0)};
// sink :: {input -> IPEncap(4, 1.1.1.1, 2.2.2.2) -> EtherEncap(0x0800, 1:1:1:1:1:1, 2:2:2:2:2:2) -> Queue -> ToDevice(eno2)};
https_filter :: {input -> Classifier(12/0800) -> Strip(14) -> CheckIPHeader2 -> IPClassifier(dst tcp port https) -> output};

td_cuckoo :: Cuckoo;
td_tee :: Tee(2);
finrst_filter :: IPClassifier(tcp opt fin or tcp opt rst);

tls_switch :: TLSSwitch;
new_cuckoo :: Cuckoo;
new_tee :: Tee(2);
elligator_check :: Elligator;
elligator_tee :: Tee(2);


source -> https_filter -> td_cuckoo -> td_tee -> sink;

td_cuckoo[1] -> tls_switch;
td_cuckoo[2] -> Discard;
td_tee[1] -> finrst_filter -> [2]td_cuckoo;


tls_switch -> [1]new_cuckoo;
tls_switch[1] -> new_cuckoo -> new_tee -> elligator_check -> elligator_tee -> sink;
tls_switch[2] -> Discard;

new_cuckoo[1] -> Discard;
new_cuckoo[2] -> Discard;
new_tee[1] -> [2]new_cuckoo;
elligator_check[1] -> Discard;
elligator_tee[1] -> [1]td_cuckoo;
