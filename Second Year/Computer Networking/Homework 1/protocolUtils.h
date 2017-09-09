#ifndef protocolUtils
#define protocolUtils

/* lungimea maxima a unui pachet si timpul maxim de asteptare pentru 
    primirea unui mesaj                                               */
unsigned char maxPackageLength = 250;
int maxWaitingTime = 5000;

unsigned char endOfBlock = 0x0D;

/* structura folosita pentru pachetele trimise */
typedef struct {
    unsigned char startOfHeader;
    unsigned char packageLength;
    unsigned char sequenceNumber;
    unsigned char packageType;
    unsigned char* data;
    unsigned short checkSum;
    unsigned char endOfBlock;

} Package;

/* structura folosita pentru pachetul de tip send-init */
typedef struct {
	unsigned char maxPackageLength;
	unsigned char packageTimeout;
	unsigned char npad;
	unsigned char padc;
	unsigned char endOfBlock;
	unsigned char qctl;
	unsigned char qbin;
	unsigned char chkt;
	unsigned char rept;
	unsigned char capa;
	unsigned char r;

} InitData;

/* initializeaza o structura de tip InitData */
InitData createInitData() {
	InitData init;
	init.maxPackageLength = 250;
	init.packageTimeout = 5;
	init.npad = 0x00;
	init.padc = 0x00;
	init.endOfBlock = 0x0D;
	init.qctl = 0x00;
	init.qbin = 0x00;
	init.chkt = 0x00;
	init.rept = 0x00;
	init.capa = 0x00;
	init.r = 0x00;

	return init;
}

/* returneaza un buffer ce contine datele dintr-o structura de tip InitData */
unsigned char* writeInitData(InitData init) {
	unsigned char* buffer = calloc(11, sizeof(unsigned char));
	if(buffer == NULL) {
		return NULL;
	}
	
	buffer[0] = init.maxPackageLength;
	buffer[1] = init.packageTimeout;
	buffer[2] = init.npad;
	buffer[3] = init.padc;
	buffer[4] = init.endOfBlock;
	buffer[5] = init.qctl;
	buffer[6] = init.qbin;
	buffer[7] = init.chkt;
	buffer[8] = init.rept;
	buffer[9] = init.capa;
	buffer[10] = init.r;
	
	return buffer;
}

/* returneaza pozitia octetilor de control intr-un mesaj continand un
   anumit pachet                                  				      */
int getCrcPosition(Package package) {
	return 4 + package.packageLength;
}

/* initializeaza o structura de tip Package */
Package createPackage(unsigned char sequenceNumber, unsigned char* data, 
					  unsigned char size, unsigned char packageType) {
	Package package;
	package.startOfHeader = 0x01;
	package.packageLength = size;
	package.sequenceNumber = sequenceNumber;
	package.packageType = packageType;
	package.data = calloc(size + 1, sizeof(char));
	if(package.data == NULL) {
		return package;
	}

	memcpy(package.data, data, size);
	package.endOfBlock = endOfBlock;

	return package;
}

/* scrie datele dintr-un pachet in payload-ul corespunzator unui mesaj */
void writePackage(Package package, msg* frame) {
	int length = 4 + package.packageLength + 3;
	char* buffer = frame->payload;

	memcpy(buffer, &package.startOfHeader, 1);
	memcpy(buffer + 1, &package.packageLength, 1);
	memcpy(buffer + 2, &package.sequenceNumber, 1);
	memcpy(buffer + 3, &package.packageType, 1);
	memcpy(buffer + 4, package.data, package.packageLength);
	
	memcpy(buffer + 4 + package.packageLength + 2, &package.endOfBlock, 1);

	frame->len = length;
}

/* scrie datele unui pachet de tip ACK in payload-ul corespunzator unui mesaj */
void writeAck(msg* frame, unsigned char sequenceNumber) {
    Package ack = createPackage(sequenceNumber, (unsigned char*)"", 0, 'Y');
    writePackage(ack, frame);
}

/* scrie datele unui pachet de tip NAK in payload-ul corespunzator unui mesaj */
void writeNak(msg* frame, unsigned char sequenceNumber) {
    Package nak = createPackage(sequenceNumber, (unsigned char*)"", 0, 'N');
    writePackage(nak, frame);
}

#endif