const { SpiralkemClient } = require('./index.js');

const c = new SpiralkemClient();
let passed = 0, total = 8;

console.log('╔════════════════════════════════════════╗');
console.log('║  Spiralkem-FHE NPM Test               ║');
console.log('╚════════════════════════════════════════╝\n');

const { pk, sk } = c.keygen();
console.log(`1. Keygen: ${pk.length === 64 && sk.length === 32 ? '✅' : '❌'}`);
if (pk.length === 64 && sk.length === 32) passed++;

const { ct, ss } = c.encaps(pk);
console.log(`2. Encaps: ${ct.length === 128 ? '✅' : '❌'}`);
if (ct.length === 128) passed++;

const ss2 = c.decaps(ct, sk);
console.log(`3. Decaps: ${ss2.length === 32 ? '✅' : '❌'}`);
if (ss2.length === 32) passed++;

console.log(`4. Match: ${Buffer.compare(ss, ss2) === 0 ? '✅' : '❌'}`);
if (Buffer.compare(ss, ss2) === 0) passed++;

const ss3 = c.decaps(ct, sk);
console.log(`5. Deterministic: ${Buffer.compare(ss2, ss3) === 0 ? '✅' : '❌'}`);
if (Buffer.compare(ss2, ss3) === 0) passed++;

const wrongSk = Buffer.alloc(32, 1);
try { c.decaps(ct, wrongSk); console.log('6. Wrong SK: ❌'); }
catch { console.log('6. Wrong SK: ✅'); passed++; }

const badCt = Buffer.concat([Buffer.from([ct[0] ^ 0xFF]), ct.slice(1)]);
try { c.decaps(badCt, sk); console.log('7. Tampered CT: ❌'); }
catch { console.log('7. Tampered CT: ✅'); passed++; }

const { pk: pk2, sk: sk2 } = c.keygen();
const { ct: ct2 } = c.encaps(pk2);
try { c.decaps(ct2, sk); console.log('8. Cross-keypair: ❌'); }
catch { console.log('8. Cross-keypair: ✅'); passed++; }

console.log(`\n╔════════════════════════════════════════╗`);
console.log(`║  RESULT: ${passed}/${total} passed ${passed === total ? '✅' : '❌'}          ║`);
console.log(`╚════════════════════════════════════════╝`);
