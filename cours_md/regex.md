Voici une décomposition détaillée des "blocs de construction" spécifiques pour grep -E et sed -E.
J'insiste sur l'option -E (Extended) car elle standardise la syntaxe. Sans elle, grep et sed se comportent différemment et c'est un cauchemar de lisibilité.
Voici les 5 familles de blocs à maîtriser pour le terminal Linux :
1. Les Ancres (Où cela se passe ?)
Ces blocs ne capturent pas de texte, ils définissent une position.
| Bloc | Nom | Fonction | Exemple grep -E |
|---|---|---|---|
| ^ | Accent circonflexe | Début de ligne. | ^Error (Lignes commençant par Error). |
| $ | Dollar | Fin de ligne. | OK$ (Lignes finissant par OK). |
| \b | Limite de mot | Frontière mot/espace (GNU Linux). | \broot\b (Le mot exact "root", pas "reboot"). |
2. Les Atomes (Quoi chercher ?)
C'est ici que sed et grep diffèrent souvent des regex web. Pour être robuste et compatible partout, privilégiez les classes POSIX ou les listes explicites plutôt que les raccourcis modernes comme \d ou \w (qui échouent souvent sous sed).
| Bloc Recommandé | Équivalent Moderne (Web) | Signification | Exemple |
|---|---|---|---|
| . | . | N'importe quel caractère. | gr.s (gras, gris, gros). |
| [0-9] | \d | Un chiffre. | [0-9]{3} (3 chiffres). |
| [a-zA-Z] | [a-zA-Z] | Une lettre. | [a-z]+ (un mot minuscule). |
| [[:alnum:]] | \w | Alphanumérique (lettres + chiffres). |  |
| [[:space:]] | \s | Espace ou tabulation. |  |
| \ | \ | Échappement (traiter le suivant comme texte). | \. (un vrai point). |
> Note de compatibilité : Sous Linux (GNU), \w et \s fonctionnent souvent. Mais si tu écris un script qui doit tourner sur macOS ou un vieux serveur Unix, utilise [0-9] et [a-z].
> 
3. Les Quantificateurs (Combien de fois ?)
Avec l'option -E, ils s'écrivent simplement.
| Bloc | Signification | Exemple |
|---|---|---|
| * | 0 ou plus (tout accepter). | .* (Absolument tout le reste de la ligne). |
| + | 1 ou plus (obligatoire). | [0-9]+ (Un nombre entier d'au moins 1 chiffre). |
| ? | 0 ou 1 (optionnel). | https? (Trouve "http" et "https"). |
| {n} | Exactement n fois. | .{10} (Exactement 10 caractères). |
| {n,m} | Entre n et m fois. | [a-z]{2,4} (Mot de 2 à 4 lettres). |
4. La Logique et Groupes
C'est ce qui donne la puissance à ta commande.
| Bloc | Nom | Fonction |
|---|---|---|
| ` | ` | OU (Pipe) |
| (...) | Groupe de capture | Isole une partie pour la réutiliser plus tard. |
| ^ (dans []) | Négation | [^0-9] (Tout caractère qui n'est pas un chiffre). |
5. La Référence Arrière (Spécial sed)
C'est le bloc magique pour le remplacement. Quand tu utilises des parenthèses () dans la partie recherche, sed numérote ce qu'il trouve.
 * \1 = Contenu de la 1ère paire de parenthèses.
 * \2 = Contenu de la 2ème paire, etc.
Exemple Ultime : Reformater un fichier CSV
Imaginons une ligne : Nom:Doe, Prenom:John
On veut : John Doe
La commande sed décomposée bloc par bloc :
sed -E 's/Nom:([a-zA-Z]+), Prenom:([a-zA-Z]+)/\2 \1/'

Analyse des blocs :
 * Nom: : Texte littéral (ancrage).
 * ( : Début capture 1.
 * [a-zA-Z]+ : Des lettres (le nom de famille).
 * ) : Fin capture 1 (stocké dans \1).
 * , Prenom: : Texte littéral de séparation.
 * ( : Début capture 2.
 * [a-zA-Z]+ : Des lettres (le prénom).
 * ) : Fin capture 2 (stocké dans \2).
 * Remplacement \2 \1 : Affiche le contenu capture 2 (Prénom), espace, contenu capture 1 (Nom).
Est-ce que cette décomposition par "rôle" t'aide à mieux visualiser la construction de la commande ?
