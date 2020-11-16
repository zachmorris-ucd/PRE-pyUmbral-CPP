from umbral import keys, signing, pre

def run_test():
    alices_private_key = keys.UmbralPrivateKey.gen_key()
    alices_public_key = alices_private_key.get_pubkey()

    alices_signing_key = keys.UmbralPrivateKey.gen_key()
    alices_verifying_key = alices_signing_key.get_pubkey()
    alices_signer = signing.Signer(private_key=alices_signing_key)

    bobs_private_key = keys.UmbralPrivateKey.gen_key()
    bobs_public_key = bobs_private_key.get_pubkey()

    plaintext= b'Proxy Re-encryption is cool!'
    ciphertext, capsule = pre.encrypt(alices_public_key, plaintext)

    cleartext = pre.decrypt(ciphertext=ciphertext, capsule=capsule, decrypting_key=alices_private_key)

    kfrags = pre.generate_kfrags(delegating_privkey=alices_private_key, signer=alices_signer, receiving_pubkey=bobs_public_key, threshold=10, N=20)

    capsule.set_correctness_keys(delegating=alices_public_key, receiving=bobs_public_key, verifying=alices_verifying_key)

    cfrags = list()
    for kfrag in kfrags[:10]:
        cfrag = pre.reencrypt(kfrag=kfrag, capsule=capsule)
        cfrags.append(cfrag)

    for cfrag in cfrags:
        capsule.attach_cfrag(cfrag)

    bob_cleartext = pre.decrypt(ciphertext=ciphertext, capsule=capsule, decrypting_key=bobs_private_key)

    print("Plain text: ", plaintext)
    print("bob_cleartext: ", bob_cleartext)

    assert bob_cleartext == plaintext
