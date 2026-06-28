declare module '@primordialomegazero/spiralkem-fhe' {
  interface Keypair { pk: Buffer; sk: Buffer; }
  interface EncapsResult { ct: Buffer; ss: Buffer; }
  
  class SpiralkemClient {
    keygen(): Keypair;
    encaps(pk: Buffer): EncapsResult;
    decaps(ct: Buffer, sk: Buffer): Buffer;
  }
  export { SpiralkemClient, Keypair, EncapsResult };
}
