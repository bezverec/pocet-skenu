# pocet-skenu
Jednoduchý program s GUI na spočítání množství skenů na OS Windows. Počítá soubory s koncovkou .tif vytvořené ve zvoleném časovém období a adresáři včetně podadresářů. 
Aplikace je vytvořena v Qt Creatoru, části kódu vygenerované ChatGPT 3.5. V roce 2025 byl použit model ChatGPT 4o mini.

Cílem bylo s žádnými nebo minimálními znalostmi vývoje softwaru a programování vytvořit klikatelný program (s grafickým rozhraním) pro zjištění údaje o počtu vytvořených skenů za určité časové období s možností časové období definovat a možností definovat cílový adresář - primární úložiště všech skenů připojené jako síťový disk na pracovních stanicích s OS Windows v digitalizačním pracovišti VKOL. Program slouží ke zjištění údaje do měsíční statistiky. V roce 2025 byl rozšířen o možnost spočítat další typy souborů typické pro datové balíčky (PSP / SIP)- výstupy z digitalizace.

**Cílové podporované verze OS Windows:** nejnovější buildy Windows podporované Microsoftem, tzn. Windows 10 & 11 s posledními updaty.

**Další informace** najdete ve [Wiki](https://github.com/bezverec/pocet-skenu/wiki) repozitáře.

**Changelog 2025:**

**2025-02-17**
- vydaná v0.2.0-alpha - přidána možnost spočítat více typů souborů s ohledem na výstupy digitalizace podle standardů NDK
- možnost vygenerovat log
- možnost spočítat soubory .txt, .xml, .jp2, .md5
- možnost spočítat PSP včetně zadání správného kódu knihovny (sigla / zkratka = počáteční název adresáře = datového balíčku = PSP / SIP)

**Program má omezení způsobená svým původem a tvůrcem. Je zamýšlen jako jednoúčelová aplikace ke splnění specifické úlohy ve velmi specifickém prostředí digitalizační workflow konkrétního pracoviště. Úlohu lze splnit mnoha způsoby lépe. Použití na vlastní nebezpečí.**

<p align="center">
<img src="https://i.ibb.co/JpWSTnj/pocet-skenu.jpg" alt="Počet skenů v0.1.0-alpha">
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/07954bf0-f2e8-45d3-b61a-553a040736c5" alt="Počet skenů v0.2.0-alpha" alt="Počet skenů v0.2.0-alpha">
</p>
