const crypto = require('crypto');

const PHI = 1.6180339887498948482;
const PK_BYTES = 64, SK_BYTES = 32, CT_BYTES = 128, SS_BYTES = 32;

class SpiralkemClient {
  constructor() { this.phi = PHI; }

  _sha256(data) { return crypto.createHash('sha256').update(data).digest(); }
  _random(bytes) { return crypto.randomBytes(bytes); }

  keygen() {
    const sk = this._random(SK_BYTES);
    const phi = Buffer.alloc(8); phi.writeDoubleBE(this.phi);
    const pk1 = this._sha256(Buffer.concat([phi, sk]));
    const pk2 = this._sha256(Buffer.concat([sk, phi]));
    return { pk: Buffer.concat([pk1, pk2]), sk };
  }

  encaps(pk) {
    const ss = this._random(SS_BYTES);
    const phi = Buffer.alloc(8); phi.writeDoubleBE(this.phi);
    const mask = this._sha256(Buffer.concat([pk, Buffer.from('encaps'), phi]));
    const ct = Buffer.alloc(CT_BYTES);
    for (let i = 0; i < 32; i++) ct[i] = ss[i] ^ mask[i];
    
    const seed = this._sha256(Buffer.concat([pk, ss]));
    let prev = seed, x = this.phi;
    for (let i = 0; i < 6; i++) {
      x = this.phi * x * (1.0 - x);
      const xbuf = Buffer.alloc(8); xbuf.writeDoubleBE(x);
      const hash = this._sha256(Buffer.concat([prev, xbuf]));
      hash.copy(ct, 32 + i * 16, 0, 16);
      prev = hash;
    }
    return { ct, ss };
  }

  decaps(ct, sk) {
    if (ct.length < CT_BYTES) throw new Error('CT too short');
    const phi = Buffer.alloc(8); phi.writeDoubleBE(this.phi);
    const pk1 = this._sha256(Buffer.concat([phi, sk]));
    const pk2 = this._sha256(Buffer.concat([sk, phi]));
    const pk = Buffer.concat([pk1, pk2]);
    
    const mask = this._sha256(Buffer.concat([pk, Buffer.from('encaps'), phi]));
    const ss = Buffer.alloc(SS_BYTES);
    for (let i = 0; i < 32; i++) ss[i] = ct[i] ^ mask[i];
    
    const seed = this._sha256(Buffer.concat([pk, ss]));
    let prev = seed, x = this.phi;
    for (let i = 0; i < 6; i++) {
      x = this.phi * x * (1.0 - x);
      const xbuf = Buffer.alloc(8); xbuf.writeDoubleBE(x);
      const hash = this._sha256(Buffer.concat([prev, xbuf]));
      if (Buffer.compare(ct.slice(32 + i * 16, 32 + i * 16 + 16), hash.slice(0, 16)) !== 0)
        throw new Error('Chain verification failed');
      prev = hash;
    }
    return ss;
  }
}

module.exports = { SpiralkemClient };
