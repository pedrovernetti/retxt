#pragma once
#ifndef _RETXT_ISO15924_INCLUDED
#define _RETXT_ISO15924_INCLUDED

#include "globalUtilities.hpp"

namespace retxt
{
	enum ISO15924ScriptCode : byte
	{
		Zxxx = 0,	// Code for unwritten documents
		Adlm = 1,	// Adlam
		Afak,		// Afaka
		Aghb,		// Caucasian Albanian
		Ahom,		// Ahom
		Arab,		// Arabic
		//Aran,		// Nastaliq Arabic
		Armi,		// Imperial Aramaic
		Armn,		// Armenian
		Avst,		// Avestan
		Bali,		// Balinese
		Bamu,		// Bamum
		Bass,		// Bassa Vah
		Batk,		// Batak
		Beng,		// Bengali (Bangla)
		Bhks,		// Bhaiksuki
		Blis,		// Blissymbols
		Bopo,		// Bopomofo
		Brah,		// Brahmi
		Brai,		// Braille
		Bugi,		// Buginese
		Buhd,		// Buhid
		Cakm,		// Chakma
		Cans,		// Unified Canadian Aboriginal Syllabics
		Cari,		// Carian
		Cham,		// Cham
		Cher,		// Cherokee
		Cirt,		// Cirth
		Copt,		// Coptic
		Cpmn,		// Cypro-Minoan
		Cprt,		// Cypriot syllabary
		Cyrl,		// Cyrillic
		//Cyrs,		// Slavonic Cyrillic
		Deva,		// Devanagari (Nagari)
		Dogr,		// Dogra
		Dsrt,		// Deseret (Mormon)
		Dupl,		// Duployan shorthand, Duployan stenography
		Egyd,		// Egyptian demotic
		Egyh,		// Egyptian hieratic
		Egyp,		// Egyptian hieroglyphs
		Elba,		// Elbasan
		Elym,		// Elymaic
		Ethi,		// Ethiopic (Ge'ez)
		Geok,		// Khutsuri (Asomtavruli and Nuskhuri)
		Geor,		// Georgian (Mkhedruli and Mtavruli)
		Glag,		// Glagolitic
		Gong,		// Gunjala Gondi
		Gonm,		// Masaram Gondi
		Goth,		// Gothic
		Gran,		// Grantha
		Grek,		// Greek
		Gujr,		// Gujarati
		Guru,		// Gurmukhi
		Hanb,		// Han + Bopomofo
		Hang,		// Hangul
		Hani,		// Han (Hanzi, Kanji, Hanja)
		Hano,		// Hanunoo
		Hans,		// Simplified Han
		Hant,		// Traditional Han
		Hatr,		// Hatran
		Hebr,		// Hebrew
		Hira,		// Hiragana
		Hluw,		// Anatolian (Luwian/Hittite) Hieroglyphs
		Hmng,		// Pahawh Hmong
		Hmnp,		// Nyiakeng Puachue Hmong
		Hrkt,		// Japanese syllabaries (Hiragana + Katakana)
		Hung,		// Old Hungarian (Hungarian Runic)
		Inds,		// Indus (Harappan)
		Ital,		// Old Italic (Etruscan, Oscan, etc.)
		Jamo,		// Jamo (Jamo subset of Hangul)
		Java,		// Javanese
		Jpan,		// Japanese (Han + Hiragana + Katakana)
		Jurc,		// Jurchen
		Kali,		// Kayah Li
		Kana,		// Katakana
		Khar,		// Kharoshthi
		Khmr,		// Khmer
		Khoj,		// Khojki
		Kitl,		// Khitan large script
		Kits,		// Khitan small script
		Knda,		// Kannada
		Kore,		// Korean (Hangul + Han)
		Kpel,		// Kpelle
		Kthi,		// Kaithi
		Lana,		// Tai Tham (Lanna)
		Laoo,		// Lao
		//Latf,		// Fraktur Latin
		//Latg,		// Gaelic Latin
		Latn,		// Latin
		Leke,		// Leke
		Lepc,		// Lepcha
		Limb,		// Limbu
		Lina,		// Linear A
		Linb,		// Linear B
		Lisu,		// Lisu (Fraser)
		Loma,		// Loma
		Lyci,		// Lycian
		Lydi,		// Lydian
		Mahj,		// Mahajani
		Maka,		// Makasar
		Mand,		// Mandaic, Mandaean
		Mani,		// Manichaean
		Marc,		// Marchen
		Maya,		// Mayan hieroglyphs
		Medf,		// Medefaidrin (Oberi Okaime)
		Mend,		// Mende Kikakui
		Merc,		// Meroitic Cursive
		Mero,		// Meroitic Hieroglyphs
		Mlym,		// Malayalam
		Modi,		// Modi
		Mong,		// Mongolian
		Moon,		// Moon (Moon code/script/type)
		Mroo,		// Mro, Mru
		Mtei,		// Meitei Mayek
		Mult,		// Multani
		Mymr,		// Myanmar (Burmese)
		Nand,		// Nandinagari
		Narb,		// Old North Arabian
		Nbat,		// Nabataean
		Newa,		// Newa, Newar, Newari, Nepala lipi
		Nkdb,		// Naxi Dongba
		Nkgb,		// Nakhi Geba
		Nkoo,		// N’Ko
		Nshu,		// Nüshu
		Ogam,		// Ogham
		Olck,		// Ol Chiki (Ol Cemet', Ol, Santali)
		Orkh,		// Old Turkic, Orkhon Runic
		Orya,		// Oriya (Odia)
		Osge,		// Osage
		Osma,		// Osmanya
		Palm,		// Palmyrene
		Pauc,		// Pau Cin Hau
		Perm,		// Old Permic
		Phag,		// Phags-pa
		Phli,		// Inscriptional Pahlavi
		Phlp,		// Psalter Pahlavi
		Phlv,		// Book Pahlavi
		Phnx,		// Phoenician
		Piqd,		// Klingon
		Plrd,		// Miao (Pollard)
		Prti,		// Inscriptional Parthian
		/*
		Qaaa .. Qabx is reserved for private use
		*/
		Rjng,		// Rejang (Redjang, Kaganga)
		Rohg,		// Hanifi Rohingya
		Roro,		// Rongorongo
		Runr,		// Runic
		Samr,		// Samaritan
		Sara,		// Sarati
		Sarb,		// Old South Arabian
		Saur,		// Saurashtra
		Sgnw,		// SignWriting
		Shaw,		// Shavian (Shaw)
		Shrd,		// Sharada
		Shui,		// Shuishu
		Sidd,		// Siddham
		Sind,		// Khudawadi, Sindhi
		Sinh,		// Sinhala
		Sogd,		// Sogdian
		Sogo,		// Old Sogdian
		Sora,		// Sora Sompeng
		Soyo,		// Soyombo
		Sund,		// Sundanese
		Sylo,		// Syloti Nagri
		Syrc,		// Syriac
		//Syre,		// Estrangelo Syriac
		//Syrj,		// Western Syriac
		//Syrn,		// Eastern Syriac
		Tagb,		// Tagbanwa
		Takr,		// Takri
		Tale,		// Tai Le
		Talu,		// New Tai Lue
		Taml,		// Tamil
		Tang,		// Tangut
		Tavt,		// Tai Viet
		Telu,		// Telugu
		Teng,		// Tengwar
		Tfng,		// Tifinagh (Berber)
		Tglg,		// Tagalog (Baybayin, Alibata)
		Thaa,		// Thaana
		Thai,		// Thai
		Tibt,		// Tibetan
		Tirh,		// Tirhuta
		Ugar,		// Ugaritic
		Vaii,		// Vai
		Visp,		// Visible Speech
		Wara,		// Warang Citi
		Wcho,		// Wancho
		Wole,		// Woleai
		Xpeo,		// Old Persian
		Xsux,		// Sumero-Akkadian Cuneiform
		Yiii,		// Yi
		Zanb,		// Zanabazar Square
		Zinh,		// Code for inherited script
		Zmth,		// Mathematical notation
		Zsym,		// Symbols
		Zsye,		// Emoji
		Zyyy,		// Code for undetermined script
		Zzzz		// Code for uncoded script
	};
}

#endif // _RETXT_ISO15924_INCLUDED
