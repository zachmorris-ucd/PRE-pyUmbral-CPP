from umbral import keys, signing, pre, config
from umbral.params import UmbralParameters
from umbral.config import default_curve

curve = default_curve()

def generate_keys():
    priv_key = keys.UmbralPrivateKey.gen_key()
    pub_key = priv_key.get_pubkey()
    priv_key_hex = priv_key.to_bytes().hex()
    pub_key_hex = pub_key.to_bytes().hex()
    # print("Private key:,", priv_key.to_bytes().hex())
    # print("Public key:,", pub_key.to_bytes().hex())
    return priv_key_hex + ", " + pub_key_hex


def encrypt_ciphertext(pub_key_str_hex, text):
    print("Inputted pub_key_str:", pub_key_str_hex)
    print("Inputted text:", text)
    pub_key = keys.UmbralPublicKey.from_bytes(bytes.fromhex(pub_key_str_hex))
    ciphertext, capsule = pre.encrypt(pub_key, text.encode("utf-8"))
    print("Capsule:", capsule.to_bytes().hex())
    print("Cipher text:", ciphertext.hex())


def decrypt_ciphertext(priv_key_str_hex, capsule_hex, ciphertext_hex):
    priv_key = keys.UmbralPrivateKey.from_bytes(bytes.fromhex(priv_key_str_hex))
    params = UmbralParameters(curve=curve)
    ciphertext = bytes.fromhex(ciphertext_hex)
    capsule = pre.Capsule.from_bytes(bytes.fromhex(capsule_hex), params)
    cleartext = pre.decrypt(ciphertext=ciphertext,
                            capsule=capsule, decrypting_key=priv_key)
    print("Clear text:", cleartext)


def run_test():
    alices_private_key_bytes = keys.UmbralPrivateKey.gen_key().to_bytes()
    alices_private_key = keys.UmbralPrivateKey.from_bytes(
        alices_private_key_bytes)
    alices_public_key = alices_private_key.get_pubkey()

    # print("Alices private key:", alices_private_key.to_bytes())
    # print("Alices public key:", alices_public_key.to_bytes())

    alices_signing_key = keys.UmbralPrivateKey.gen_key()
    alices_verifying_key = alices_signing_key.get_pubkey()
    alices_signer = signing.Signer(private_key=alices_signing_key)

    bobs_private_key = keys.UmbralPrivateKey.gen_key()
    bobs_public_key = bobs_private_key.get_pubkey()

    plaintext = b'Proxy Re-encryption is cool!'
    ciphertext, capsule = pre.encrypt(alices_public_key, plaintext)

    cleartext = pre.decrypt(ciphertext=ciphertext,
                            capsule=capsule,
                            decrypting_key=alices_private_key)

    kfrags = pre.generate_kfrags(delegating_privkey=alices_private_key,
                                 signer=alices_signer,
                                 receiving_pubkey=bobs_public_key, threshold=10,
                                 N=20)

    capsule.set_correctness_keys(delegating=alices_public_key,
                                 receiving=bobs_public_key,
                                 verifying=alices_verifying_key)

    cfrags = list()
    for kfrag in kfrags[:10]:
        cfrag = pre.reencrypt(kfrag=kfrag, capsule=capsule)
        cfrags.append(cfrag)

    for cfrag in cfrags:
        capsule.attach_cfrag(cfrag)

    bob_cleartext = pre.decrypt(ciphertext=ciphertext, capsule=capsule,
                                decrypting_key=bobs_private_key)

    print("Plain text: ", plaintext)
    print("bob_cleartext: ", bob_cleartext)

    assert bob_cleartext == plaintext
