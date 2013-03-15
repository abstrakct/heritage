/*
 * Roguelike Engine in C++
 *
 * NPC specific stuff.
 *
 */

using namespace std;

#include <iostream>

#include "libtcod.hpp"
#include "display.h"
#include "actor.h"
#include "npc.h"
#include "common.h"
#include "world.h"

extern Display *display;
extern World *world;

const char *malegivenname[] = {
"Aaron", "Abdul", "Abe", "Abel", "Abraham", "Abram", "Adalberto", "Adam", "Adan", "Adolfo", "Adolph", "Adrian", "Agustin",
"Ahmad", "Ahmed", "Al", "Alan", "Albert", "Alberto", "Alden", "Aldo", "Alec", "Alejandro", "Alex", "Alexander", "Alexis",
"Alfonso", "Alfonzo", "Alfred", "Alfredo", "Ali", "Allan", "Allen", "Alonso", "Alonzo", "Alphonse", "Alphonso", "Alton", "Alva",
"Alvaro", "Alvin", "Amado", "Ambrose", "Amos", "Anderson", "Andre", "Andrea", "Andreas", "Andres", "Andrew", "Andy", "Angel",
"Angelo", "Anibal", "Anthony", "Antione", "Antoine", "Anton", "Antone", "Antonia", "Antonio", "Antony", "Antwan", "Archie",
"Arden", "Ariel", "Arlen", "Arlie", "Armand", "Armando", "Arnold", "Arnoldo", "Arnulfo", "Aron", "Arron", "Art", "Arthur",
"Arturo", "Asa", "Ashley", "Aubrey", "August", "Augustine", "Augustus", "Aurelio", "Austin", "Avery", "Barney", "Barrett",
"Barry", "Bart", "Barton", "Basil", "Beau", "Ben", "Benedict", "Benito", "Benjamin", "Bennett", "Bennie", "Benny", "Benton",
"Bernard", "Bernardo", "Bernie", "Berry", "Bert", "Bertram", "Bill", "Billie", "Billy", "Blaine", "Blair", "Blake", "Bo", "Bob",
"Bobbie", "Bobby", "Booker", "Boris", "Boyce", "Boyd", "Brad", "Bradford", "Bradley", "Bradly", "Brady", "Brain", "Branden",
"Brandon", "Brant", "Brendan", "Brendon", "Brent", "Brenton", "Bret", "Brett", "Brian", "Brice", "Britt", "Brock", "Broderick",
"Brooks", "Bruce", "Bruno", "Bryan", "Bryant", "Bryce", "Bryon", "Buck", "Bud", "Buddy", "Buford", "Burl", "Burt", "Burton",
"Buster", "Byron", "Caleb", "Calvin", "Cameron", "Carey", "Carl", "Carlo", "Carlos", "Carlton", "Carmelo", "Carmen", "Carmine",
"Carol", "Carrol", "Carroll", "Carson", "Carter", "Cary", "Casey", "Cecil", "Cedric", "Cedrick", "Cesar", "Chad", "Chadwick",
"Chance", "Chang", "Charles", "Charley", "Charlie", "Chas", "Chase", "Chauncey", "Chester", "Chet", "Chi", "Chong", "Chris",
"Christian", "Christoper", "Christopher", "Chuck", "Chung", "Clair", "Clarence", "Clark", "Claud", "Claude", "Claudio", "Clay",
"Clayton", "Clement", "Clemente", "Cleo", "Cletus", "Cleveland", "Cliff", "Clifford", "Clifton", "Clint", "Clinton", "Clyde",
"Cody", "Colby", "Cole", "Coleman", "Colin", "Collin", "Colton", "Columbus", "Connie", "Conrad", "Cordell", "Corey", "Cornelius",
"Cornell", "Cortez", "Cory", "Courtney", "Coy", "Craig", "Cristobal", "Cristopher", "Cruz", "Curt", "Curtis", "Cyril", "Cyrus",
"Dale", "Dallas", "Dalton", "Damian", "Damien", "Damion", "Damon", "Dan", "Dana", "Dane", "Danial", "Daniel", "Danilo", "Dannie",
"Danny", "Dante", "Darell", "Daren", "Darin", "Dario", "Darius", "Darnell", "Daron", "Darrel", "Darrell", "Darren", "Darrick",
"Darrin", "Darron", "Darryl", "Darwin", "Daryl", "Dave", "David", "Davis", "Dean", "Deandre", "Deangelo", "Dee", "Del", "Delbert",
"Delmar", "Delmer", "Demarcus", "Demetrius", "Denis", "Dennis", "Denny", "Denver", "Deon", "Derek", "Derick", "Derrick",
"Deshawn", "Desmond", "Devin", "Devon", "Dewayne", "Dewey", "Dewitt", "Dexter", "Dick", "Diego", "Dillon", "Dino", "Dion", "Dirk",
"Domenic", "Domingo", "Dominic", "Dominick", "Dominique", "Don", "Donald", "Dong", "Donn", "Donnell", "Donnie", "Donny",
"Donovan", "Donte", "Dorian", "Dorsey", "Doug", "Douglas", "Douglass", "Doyle", "Drew", "Duane", "Dudley", "Duncan", "Dustin",
"Dusty", "Dwain", "Dwayne", "Dwight", "Dylan", "Earl", "Earle", "Earnest", "Ed", "Eddie", "Eddy", "Edgar", "Edgardo", "Edison",
"Edmond", "Edmund", "Edmundo", "Eduardo", "Edward", "Edwardo", "Edwin", "Efrain", "Efren", "Elbert", "Elden", "Eldon", "Eldridge",
"Eli", "Elias", "Elijah", "Eliseo", "Elisha", "Elliot", "Elliott", "Ellis", "Ellsworth", "Elmer", "Elmo", "Eloy", "Elroy",
"Elton", "Elvin", "Elvis", "Elwood", "Emanuel", "Emerson", "Emery", "Emil", "Emile", "Emilio", "Emmanuel", "Emmett", "Emmitt",
"Emory", "Enoch", "Enrique", "Erasmo", "Eric", "Erich", "Erick", "Erik", "Erin", "Ernest", "Ernesto", "Ernie", "Errol", "Ervin",
"Erwin", "Esteban", "Ethan", "Eugene", "Eugenio", "Eusebio", "Evan", "Everett", "Everette", "Ezekiel", "Ezequiel", "Ezra",
"Fabian", "Faustino", "Fausto", "Federico", "Felipe", "Felix", "Felton", "Ferdinand", "Fermin", "Fernando", "Fidel", "Filiberto",
"Fletcher", "Florencio", "Florentino", "Floyd", "Forest", "Forrest", "Foster", "Frances", "Francesco", "Francis", "Francisco",
"Frank", "Frankie", "Franklin", "Franklyn", "Fred", "Freddie", "Freddy", "Frederic", "Frederick", "Fredric", "Fredrick",
"Freeman", "Fritz", "Gabriel", "Gail", "Gale", "Galen", "Garfield", "Garland", "Garret", "Garrett", "Garry", "Garth", "Gary",
"Gaston", "Gavin", "Gayle", "Gaylord", "Genaro", "Gene", "Geoffrey", "George", "Gerald", "Geraldo", "Gerard", "Gerardo", "German",
"Gerry", "Gil", "Gilbert", "Gilberto", "Gino", "Giovanni", "Giuseppe", "Glen", "Glenn", "Gonzalo", "Gordon", "Grady", "Graham",
"Graig", "Grant", "Granville", "Greg", "Gregg", "Gregorio", "Gregory", "Grover", "Guadalupe", "Guillermo", "Gus", "Gustavo",
"Guy", "Hai", "Hal", "Hank", "Hans", "Harlan", "Harland", "Harley", "Harold", "Harris", "Harrison", "Harry", "Harvey", "Hassan",
"Hayden", "Haywood", "Heath", "Hector", "Henry", "Herb", "Herbert", "Heriberto", "Herman", "Herschel", "Hershel", "Hilario",
"Hilton", "Hipolito", "Hiram", "Hobert", "Hollis", "Homer", "Hong", "Horace", "Horacio", "Hosea", "Houston", "Howard", "Hoyt",
"Hubert", "Huey", "Hugh", "Hugo", "Humberto", "Hung", "Hunter", "Hyman", "Ian", "Ignacio", "Ike", "Ira", "Irvin", "Irving",
"Irwin", "Isaac", "Isaiah", "Isaias", "Isiah", "Isidro", "Ismael", "Israel", "Isreal", "Issac", "Ivan", "Ivory", "Jacinto",
"Jack", "Jackie", "Jackson", "Jacob", "Jacques", "Jae", "Jaime", "Jake", "Jamaal", "Jamal", "Jamar", "Jame", "Jamel", "James",
"Jamey", "Jamie", "Jamison", "Jan", "Jared", "Jarod", "Jarred", "Jarrett", "Jarrod", "Jarvis", "Jason", "Jasper", "Javier", "Jay",
"Jayson", "Jc", "Jean", "Jed", "Jeff", "Jefferey", "Jefferson", "Jeffery", "Jeffrey", "Jeffry", "Jerald", "Jeramy", "Jere",
"Jeremiah", "Jeremy", "Jermaine", "Jerold", "Jerome", "Jeromy", "Jerrell", "Jerrod", "Jerrold", "Jerry", "Jess", "Jesse",
"Jessie", "Jesus", "Jewel", "Jewell", "Jim", "Jimmie", "Jimmy", "Joan", "Joaquin", "Jody", "Joe", "Joel", "Joesph", "Joey",
"John", "Johnathan", "Johnathon", "Johnie", "Johnnie", "Johnny", "Johnson", "Jon", "Jonah", "Jonas", "Jonathan", "Jonathon",
"Jordan", "Jordon", "Jorge", "Jose", "Josef", "Joseph", "Josh", "Joshua", "Josiah", "Jospeh", "Josue", "Juan", "Jude", "Judson",
"Jules", "Julian", "Julio", "Julius", "Junior", "Justin", "Kareem", "Karl", "Kasey", "Keenan", "Keith", "Kelley", "Kelly",
"Kelvin", "Ken", "Kendall", "Kendrick", "Keneth", "Kenneth", "Kennith", "Kenny", "Kent", "Kenton", "Kermit", "Kerry", "Keven",
"Kevin", "Kieth", "Kim", "King", "Kip", "Kirby", "Kirk", "Korey", "Kory", "Kraig", "Kris", "Kristofer", "Kristopher", "Kurt",
"Kurtis", "Kyle", "Lacy", "Lamar", "Lamont", "Lance", "Landon", "Lane", "Lanny", "Larry", "Lauren", "Laurence", "Lavern",
"Laverne", "Lawerence", "Lawrence", "Lazaro", "Leandro", "Lee", "Leif", "Leigh", "Leland", "Lemuel", "Len", "Lenard", "Lenny",
"Leo", "Leon", "Leonard", "Leonardo", "Leonel", "Leopoldo", "Leroy", "Les", "Lesley", "Leslie", "Lester", "Levi", "Lewis",
"Lincoln", "Lindsay", "Lindsey", "Lino", "Linwood", "Lionel", "Lloyd", "Logan", "Lon", "Long", "Lonnie", "Lonny", "Loren",
"Lorenzo", "Lou", "Louie", "Louis", "Lowell", "Loyd", "Lucas", "Luciano", "Lucien", "Lucio", "Lucius", "Luigi", "Luis", "Luke",
"Lupe", "Luther", "Lyle", "Lyman", "Lyndon", "Lynn", "Lynwood", "Mac", "Mack", "Major", "Malcolm", "Malcom", "Malik", "Man",
"Manual", "Manuel", "Marc", "Marcel", "Marcelino", "Marcellus", "Marcelo", "Marco", "Marcos", "Marcus", "Margarito", "Maria",
"Mariano", "Mario", "Marion", "Mark", "Markus", "Marlin", "Marlon", "Marquis", "Marshall", "Martin", "Marty", "Marvin", "Mary",
"Mason", "Mathew", "Matt", "Matthew", "Maurice", "Mauricio", "Mauro", "Max", "Maximo", "Maxwell", "Maynard", "Mckinley", "Mel",
"Melvin", "Merle", "Merlin", "Merrill", "Mervin", "Micah", "Michael", "Michal", "Michale", "Micheal", "Michel", "Mickey",
"Miguel", "Mike", "Mikel", "Milan", "Miles", "Milford", "Millard", "Milo", "Milton", "Minh", "Miquel", "Mitch", "Mitchel",
"Mitchell", "Modesto", "Mohamed", "Mohammad", "Mohammed", "Moises", "Monroe", "Monte", "Monty", "Morgan", "Morris", "Morton",
"Mose", "Moses", "Moshe", "Murray", "Myles", "Myron", "Napoleon", "Nathan", "Nathanael", "Nathanial", "Nathaniel", "Neal", "Ned",
"Neil", "Nelson", "Nestor", "Neville", "Newton", "Nicholas", "Nick", "Nickolas", "Nicky", "Nicolas", "Nigel", "Noah", "Noble",
"Noe", "Noel", "Nolan", "Norbert", "Norberto", "Norman", "Normand", "Norris", "Numbers", "Octavio", "Odell", "Odis", "Olen",
"Olin", "Oliver", "Ollie", "Omar", "Omer", "Oren", "Orlando", "Orval", "Orville", "Oscar", "Osvaldo", "Oswaldo", "Otha", "Otis",
"Otto", "Owen", "Pablo", "Palmer", "Paris", "Parker", "Pasquale", "Pat", "Patricia", "Patrick", "Paul", "Pedro", "Percy", "Perry",
"Pete", "Peter", "Phil", "Philip", "Phillip", "Pierre", "Porfirio", "Porter", "Preston", "Prince", "Quentin", "Quincy", "Quinn",
"Quintin", "Quinton", "Rafael", "Raleigh", "Ralph", "Ramiro", "Ramon", "Randal", "Randall", "Randell", "Randolph", "Randy",
"Raphael", "Rashad", "Raul", "Ray", "Rayford", "Raymon", "Raymond", "Raymundo", "Reed", "Refugio", "Reggie", "Reginald", "Reid",
"Reinaldo", "Renaldo", "Renato", "Rene", "Reuben", "Rex", "Rey", "Reyes", "Reynaldo", "Rhett", "Ricardo", "Rich", "Richard",
"Richie", "Rick", "Rickey", "Rickie", "Ricky", "Rico", "Rigoberto", "Riley", "Rob", "Robbie", "Robby", "Robert", "Roberto",
"Robin", "Robt", "Rocco", "Rocky", "Rod", "Roderick", "Rodger", "Rodney", "Rodolfo", "Rodrick", "Rodrigo", "Rogelio", "Roger",
"Roland", "Rolando", "Rolf", "Rolland", "Roman", "Romeo", "Ron", "Ronald", "Ronnie", "Ronny", "Roosevelt", "Rory", "Rosario",
"Roscoe", "Rosendo", "Ross", "Roy", "Royal", "Royce", "Ruben", "Rubin", "Rudolf", "Rudolph", "Rudy", "Rueben", "Rufus", "Rupert",
"Russ", "Russel", "Russell", "Rusty", "Ryan", "Sal", "Salvador", "Salvatore", "Sam", "Sammie", "Sammy", "Samual", "Samuel",
"Sandy", "Sanford", "Sang", "Santiago", "Santo", "Santos", "Saul", "Scot", "Scott", "Scottie", "Scotty", "Sean", "Sebastian",
"Sergio", "Seth", "Seymour", "Shad", "Shane", "Shannon", "Shaun", "Shawn", "Shayne", "Shelby", "Sheldon", "Shelton", "Sherman",
"Sherwood", "Shirley", "Shon", "Sid", "Sidney", "Silas", "Simon", "Sol", "Solomon", "Son", "Sonny", "Spencer", "Stacey", "Stacy",
"Stan", "Stanford", "Stanley", "Stanton", "Stefan", "Stephan", "Stephen", "Sterling", "Steve", "Steven", "Stevie", "Stewart",
"Stuart", "Sung", "Sydney", "Sylvester", "Tad", "Tanner", "Taylor", "Ted", "Teddy", "Teodoro", "Terence", "Terrance", "Terrell",
"Terrence", "Terry", "Thad", "Thaddeus", "Thanh", "Theo", "Theodore", "Theron", "Thomas", "Thurman", "Tim", "Timmy", "Timothy",
"Titus", "Tobias", "Toby", "Tod", "Todd", "Tom", "Tomas", "Tommie", "Tommy", "Toney", "Tony", "Tory", "Tracey", "Tracy", "Travis",
"Trent", "Trenton", "Trevor", "Trey", "Trinidad", "Tristan", "Troy", "Truman", "Tuan", "Ty", "Tyler", "Tyree", "Tyrell", "Tyron",
"Tyrone", "Tyson", "Ulysses", "Val", "Valentin", "Valentine", "Van", "Vance", "Vaughn", "Vern", "Vernon", "Vicente", "Victor",
"Vince", "Vincent", "Vincenzo", "Virgil", "Virgilio", "Vito", "Von", "Wade", "Waldo", "Walker", "Wallace", "Wally", "Walter",
"Walton", "Ward", "Warner", "Warren", "Waylon", "Wayne", "Weldon", "Wendell", "Werner", "Wes", "Wesley", "Weston", "Whitney",
"Wilber", "Wilbert", "Wilbur", "Wilburn", "Wiley", "Wilford", "Wilfred", "Wilfredo", "Will", "Willard", "William", "Williams",
"Willian", "Willie", "Willis", "Willy", "Wilmer", "Wilson", "Wilton", "Winford", "Winfred", "Winston", "Woodrow", "Wyatt",
"Xavier", "Yong", "Young", "Zachariah", "Zachary", "Zachery", "Zack", "Zackary", "Zane"
};

const char *femalegivenname[] = {
"Aaron", "Abbey", "Abbie", "Abby", "Abigail", "Ada", "Adah", "Adaline", "Adam", "Addie", "Adela", "Adelaida", "Adelaide", "Adele",
"Adelia", "Adelina", "Adeline", "Adell", "Adella", "Adelle", "Adena", "Adina", "Adria", "Adrian", "Adriana", "Adriane",
"Adrianna", "Adrianne", "Adrien", "Adriene", "Adrienne", "Afton", "Agatha", "Agnes", "Agnus", "Agripina", "Agustina", "Ai",
"Aida", "Aide", "Aiko", "Aileen", "Ailene", "Aimee", "Aisha", "Aja", "Akiko", "Akilah", "Alaina", "Alaine", "Alana", "Alane",
"Alanna", "Alayna", "Alba", "Albert", "Alberta", "Albertha", "Albertina", "Albertine", "Albina", "Alda", "Alease", "Alecia",
"Aleen", "Aleida", "Aleisha", "Alejandra", "Alejandrina", "Alena", "Alene", "Alesha", "Aleshia", "Alesia", "Alessandra", "Aleta",
"Aletha", "Alethea", "Alethia", "Alex", "Alexa", "Alexander", "Alexandra", "Alexandria", "Alexia", "Alexis", "Alfreda",
"Alfredia", "Ali", "Alia", "Alica", "Alice", "Alicia", "Alida", "Alina", "Aline", "Alisa", "Alise", "Alisha", "Alishia", "Alisia",
"Alison", "Alissa", "Alita", "Alix", "Aliza", "Alla", "Alleen", "Allegra", "Allen", "Allena", "Allene", "Allie", "Alline",
"Allison", "Allyn", "Allyson", "Alma", "Almeda", "Almeta", "Alona", "Alpha", "Alta", "Altagracia", "Altha", "Althea", "Alva",
"Alvera", "Alverta", "Alvina", "Alyce", "Alycia", "Alyse", "Alysha", "Alysia", "Alyson", "Alyssa", "Amada", "Amal", "Amalia",
"Amanda", "Amber", "Amberly", "Amee", "Amelia", "America", "Ami", "Amie", "Amiee", "Amina", "Amira", "Ammie", "Amparo", "Amy",
"An", "Ana", "Anabel", "Analisa", "Anamaria", "Anastacia", "Anastasia", "Andera", "Andra", "Andre", "Andrea", "Andree", "Andrew",
"Andria", "Anette", "Angel", "Angela", "Angele", "Angelena", "Angeles", "Angelia", "Angelic", "Angelica", "Angelika", "Angelina",
"Angeline", "Angelique", "Angelita", "Angella", "Angelo", "Angelyn", "Angie", "Angila", "Angla", "Angle", "Anglea", "Anh",
"Anika", "Anisa", "Anisha", "Anissa", "Anita", "Anitra", "Anja", "Anjanette", "Anjelica", "Ann", "Anna", "Annabel", "Annabell",
"Annabelle", "Annalee", "Annalisa", "Annamae", "Annamaria", "Annamarie", "Anne", "Anneliese", "Annelle", "Annemarie", "Annett",
"Annetta", "Annette", "Annice", "Annie", "Annika", "Annis", "Annita", "Annmarie", "Anthony", "Antionette", "Antoinette",
"Antonetta", "Antonette", "Antonia", "Antonietta", "Antonina", "Antonio", "Anya", "Apolonia", "April", "Apryl", "Ara", "Araceli",
"Aracelis", "Aracely", "Arcelia", "Ardath", "Ardelia", "Ardell", "Ardella", "Ardelle", "Ardis", "Ardith", "Aretha", "Argelia",
"Argentina", "Ariana", "Ariane", "Arianna", "Arianne", "Arica", "Arie", "Ariel", "Arielle", "Arla", "Arlean", "Arleen", "Arlena",
"Arlene", "Arletha", "Arletta", "Arlette", "Arlinda", "Arline", "Arlyne", "Armanda", "Armandina", "Armida", "Arminda", "Arnetta",
"Arnette", "Arnita", "Arthur", "Artie", "Arvilla", "Asha", "Ashanti", "Ashely", "Ashlea", "Ashlee", "Ashleigh", "Ashley", "Ashli",
"Ashlie", "Ashly", "Ashlyn", "Ashton", "Asia", "Asley", "Assunta", "Astrid", "Asuncion", "Athena", "Aubrey", "Audie", "Audra",
"Audrea", "Audrey", "Audria", "Audrie", "Audry", "Augusta", "Augustina", "Augustine", "Aundrea", "Aura", "Aurea", "Aurelia",
"Aurora", "Aurore", "Austin", "Autumn", "Ava", "Avelina", "Avery", "Avis", "Avril", "Awilda", "Ayako", "Ayana", "Ayanna",
"Ayesha", "Azalee", "Azucena", "Azzie", "Babara", "Babette", "Bailey", "Bambi", "Bao", "Barabara", "Barb", "Barbar", "Barbara",
"Barbera", "Barbie", "Barbra", "Bari", "Barrie", "Basilia", "Bea", "Beata", "Beatrice", "Beatris", "Beatriz", "Beaulah", "Bebe",
"Becki", "Beckie", "Becky", "Bee", "Belen", "Belia", "Belinda", "Belkis", "Bell", "Bella", "Belle", "Belva", "Benita", "Bennie",
"Berenice", "Berna", "Bernadette", "Bernadine", "Bernarda", "Bernardina", "Bernardine", "Berneice", "Bernetta", "Bernice",
"Bernie", "Berniece", "Bernita", "Berry", "Berta", "Bertha", "Bertie", "Beryl", "Bess", "Bessie", "Beth", "Bethanie", "Bethann",
"Bethany", "Bethel", "Betsey", "Betsy", "Bette", "Bettie", "Bettina", "Betty", "Bettyann", "Bettye", "Beula", "Beulah", "Bev",
"Beverlee", "Beverley", "Beverly", "Bianca", "Bibi", "Billi", "Billie", "Billy", "Billye", "Birdie", "Birgit", "Blair", "Blake",
"Blanca", "Blanch", "Blanche", "Blondell", "Blossom", "Blythe", "Bobbi", "Bobbie", "Bobby", "Bobbye", "Bobette", "Bok", "Bong",
"Bonita", "Bonnie", "Bonny", "Branda", "Brande", "Brandee", "Brandi", "Brandie", "Brandon", "Brandy", "Breana", "Breann",
"Breanna", "Breanne", "Bree", "Brenda", "Brenna", "Brett", "Brian", "Briana", "Brianna", "Brianne", "Bridget", "Bridgett",
"Bridgette", "Brigette", "Brigid", "Brigida", "Brigitte", "Brinda", "Britany", "Britney", "Britni", "Britt", "Britta",
"Brittaney", "Brittani", "Brittanie", "Brittany", "Britteny", "Brittney", "Brittni", "Brittny", "Bronwyn", "Brook", "Brooke",
"Bruna", "Brunilda", "Bryanna", "Brynn", "Buena", "Buffy", "Bula", "Bulah", "Bunny", "Burma", "Caitlin", "Caitlyn", "Calandra",
"Calista", "Callie", "Camelia", "Camellia", "Cameron", "Cami", "Camie", "Camila", "Camilla", "Camille", "Cammie", "Cammy",
"Candace", "Candance", "Candelaria", "Candi", "Candice", "Candida", "Candie", "Candis", "Candra", "Candy", "Candyce", "Caprice",
"Cara", "Caren", "Carey", "Cari", "Caridad", "Carie", "Carin", "Carina", "Carisa", "Carissa", "Carita", "Carl", "Carla", "Carlee",
"Carleen", "Carlena", "Carlene", "Carletta", "Carley", "Carli", "Carlie", "Carline", "Carlita", "Carlos", "Carlota", "Carlotta",
"Carly", "Carlyn", "Carma", "Carman", "Carmel", "Carmela", "Carmelia", "Carmelina", "Carmelita", "Carmella", "Carmen", "Carmina",
"Carmon", "Carol", "Carola", "Carolann", "Carole", "Carolee", "Carolin", "Carolina", "Caroline", "Caroll", "Carolyn", "Carolyne",
"Carolynn", "Caron", "Caroyln", "Carri", "Carrie", "Carrol", "Carroll", "Carry", "Cary", "Caryl", "Carylon", "Caryn", "Casandra",
"Casey", "Casie", "Casimira", "Cassandra", "Cassaundra", "Cassey", "Cassi", "Cassidy", "Cassie", "Cassondra", "Cassy", "Catalina",
"Catarina", "Caterina", "Catharine", "Catherin", "Catherina", "Catherine", "Cathern", "Catheryn", "Cathey", "Cathi", "Cathie",
"Cathleen", "Cathrine", "Cathryn", "Cathy", "Catina", "Catrice", "Catrina", "Cayla", "Cecelia", "Cecil", "Cecila", "Cecile",
"Cecilia", "Cecille", "Cecily", "Celena", "Celesta", "Celeste", "Celestina", "Celestine", "Celia", "Celina", "Celinda", "Celine",
"Celsa", "Ceola", "Chae", "Chan", "Chana", "Chanda", "Chandra", "Chanel", "Chanell", "Chanelle", "Chang", "Chantal", "Chantay",
"Chante", "Chantel", "Chantell", "Chantelle", "Chara", "Charis", "Charise", "Charissa", "Charisse", "Charita", "Charity",
"Charla", "Charleen", "Charlena", "Charlene", "Charles", "Charlesetta", "Charlette", "Charlie", "Charline", "Charlott",
"Charlotte", "Charlsie", "Charlyn", "Charmain", "Charmaine", "Charolette", "Chasidy", "Chasity", "Chassidy", "Chastity", "Chau",
"Chaya", "Chelsea", "Chelsey", "Chelsie", "Cher", "Chere", "Cheree", "Cherelle", "Cheri", "Cherie", "Cherilyn", "Cherise",
"Cherish", "Cherly", "Cherlyn", "Cherri", "Cherrie", "Cherry", "Cherryl", "Chery", "Cheryl", "Cheryle", "Cheryll", "Cheyenne",
"Chi", "Chia", "Chieko", "Chin", "China", "Ching", "Chiquita", "Chloe", "Chong", "Chris", "Chrissy", "Christa", "Christal",
"Christeen", "Christel", "Christen", "Christena", "Christene", "Christi", "Christia", "Christian", "Christiana", "Christiane",
"Christie", "Christin", "Christina", "Christine", "Christinia", "Christopher", "Christy", "Chrystal", "Chu", "Chun", "Chung",
"Ciara", "Cicely", "Ciera", "Cierra", "Cinda", "Cinderella", "Cindi", "Cindie", "Cindy", "Cinthia", "Cira", "Clair", "Claire",
"Clara", "Clare", "Clarence", "Claretha", "Claretta", "Claribel", "Clarice", "Clarinda", "Clarine", "Claris", "Clarisa",
"Clarissa", "Clarita", "Classie", "Claude", "Claudette", "Claudia", "Claudie", "Claudine", "Clelia", "Clemencia", "Clementina",
"Clementine", "Clemmie", "Cleo", "Cleopatra", "Cleora", "Cleotilde", "Cleta", "Clora", "Clorinda", "Clotilde", "Clyde", "Codi",
"Cody", "Colby", "Coleen", "Colene", "Coletta", "Colette", "Colleen", "Collen", "Collene", "Collette", "Concepcion", "Conception",
"Concetta", "Concha", "Conchita", "Connie", "Constance", "Consuela", "Consuelo", "Contessa", "Cora", "Coral", "Coralee",
"Coralie", "Corazon", "Cordelia", "Cordia", "Cordie", "Coreen", "Corene", "Coretta", "Corey", "Cori", "Corie", "Corina", "Corine",
"Corinna", "Corinne", "Corliss", "Cornelia", "Corrie", "Corrin", "Corrina", "Corrine", "Corrinne", "Cortney", "Cory", "Courtney",
"Creola", "Cris", "Criselda", "Crissy", "Crista", "Cristal", "Cristen", "Cristi", "Cristie", "Cristin", "Cristina", "Cristine",
"Cristy", "Cruz", "Crysta", "Crystal", "Crystle", "Cuc", "Curtis", "Cyndi", "Cyndy", "Cynthia", "Cyrstal", "Cythia", "Dacia",
"Dagmar", "Dagny", "Dahlia", "Daina", "Daine", "Daisey", "Daisy", "Dakota", "Dale", "Dalene", "Dalia", "Dalila", "Dallas",
"Damaris", "Dan", "Dana", "Danae", "Danelle", "Danette", "Dani", "Dania", "Danica", "Daniel", "Daniela", "Daniele", "Daniell",
"Daniella", "Danielle", "Danika", "Danille", "Danita", "Dann", "Danna", "Dannette", "Dannie", "Dannielle", "Danuta", "Danyel",
"Danyell", "Danyelle", "Daphine", "Daphne", "Dara", "Darby", "Darcel", "Darcey", "Darci", "Darcie", "Darcy", "Daria", "Darla",
"Darleen", "Darlena", "Darlene", "Darline", "Darnell", "Daryl", "David", "Davida", "Davina", "Dawn", "Dawna", "Dawne", "Dayle",
"Dayna", "Daysi", "Deadra", "Dean", "Deana", "Deandra", "Deandrea", "Deane", "Deann", "Deanna", "Deanne", "Deb", "Debbi",
"Debbie", "Debbra", "Debby", "Debera", "Debi", "Debora", "Deborah", "Debra", "Debrah", "Debroah", "Dede", "Dedra", "Dee",
"Deeann", "Deeanna", "Deedee", "Deedra", "Deena", "Deetta", "Deidra", "Deidre", "Deirdre", "Deja", "Delaine", "Delana", "Delcie",
"Delena", "Delfina", "Delia", "Delicia", "Delila", "Delilah", "Delinda", "Delisa", "Dell", "Della", "Delma", "Delmy", "Delois",
"Deloise", "Delora", "Deloras", "Delores", "Deloris", "Delorse", "Delpha", "Delphia", "Delphine", "Delsie", "Delta", "Demetra",
"Demetria", "Demetrice", "Demetrius", "Dena", "Denae", "Deneen", "Denese", "Denice", "Denise", "Denisha", "Denisse", "Denita",
"Denna", "Dennis", "Dennise", "Denny", "Denyse", "Deon", "Deonna", "Desirae", "Desire", "Desiree", "Despina", "Dessie", "Destiny",
"Detra", "Devin", "Devon", "Devona", "Devora", "Devorah", "Dia", "Diamond", "Dian", "Diana", "Diane", "Diann", "Dianna", "Dianne",
"Diedra", "Diedre", "Dierdre", "Digna", "Dimple", "Dina", "Dinah", "Dinorah", "Dion", "Dione", "Dionna", "Dionne", "Divina",
"Dixie", "Dodie", "Dollie", "Dolly", "Dolores", "Doloris", "Domenica", "Dominga", "Dominica", "Dominique", "Dominque", "Domitila",
"Domonique", "Dona", "Donald", "Donella", "Donetta", "Donette", "Dong", "Donita", "Donna", "Donnetta", "Donnette", "Donnie",
"Donya", "Dora", "Dorathy", "Dorcas", "Doreatha", "Doreen", "Dorene", "Doretha", "Dorethea", "Doretta", "Dori", "Doria", "Dorian",
"Dorie", "Dorinda", "Dorine", "Doris", "Dorla", "Dorotha", "Dorothea", "Dorothy", "Dorris", "Dortha", "Dorthea", "Dorthey",
"Dorthy", "Dot", "Dottie", "Dotty", "Dovie", "Dreama", "Drema", "Drew", "Drucilla", "Drusilla", "Dulce", "Dulcie", "Dung",
"Dusti", "Dusty", "Dwana", "Dyan", "Earlean", "Earleen", "Earlene", "Earlie", "Earline", "Earnestine", "Eartha", "Easter",
"Eboni", "Ebonie", "Ebony", "Echo", "Eda", "Edda", "Eddie", "Edelmira", "Eden", "Edie", "Edith", "Edna", "Edra", "Edris",
"Edward", "Edwina", "Edyth", "Edythe", "Effie", "Ehtel", "Eileen", "Eilene", "Ela", "Eladia", "Elaina", "Elaine", "Elana",
"Elane", "Elanor", "Elayne", "Elba", "Elda", "Eldora", "Eleanor", "Eleanora", "Eleanore", "Elena", "Elene", "Eleni", "Elenor",
"Elenora", "Elenore", "Eleonor", "Eleonora", "Eleonore", "Elfreda", "Elfrieda", "Elfriede", "Elia", "Eliana", "Elicia", "Elida",
"Elidia", "Elin", "Elina", "Elinor", "Elinore", "Elisa", "Elisabeth", "Elise", "Elisha", "Elissa", "Eliz", "Eliza", "Elizabet",
"Elizabeth", "Elizbeth", "Elizebeth", "Elke", "Ella", "Ellamae", "Ellan", "Ellen", "Ellena", "Elli", "Ellie", "Ellis", "Elly",
"Ellyn", "Elma", "Elmer", "Elna", "Elnora", "Elodia", "Elois", "Eloisa", "Eloise", "Elouise", "Elsa", "Else", "Elsie", "Elsy",
"Elva", "Elvera", "Elvia", "Elvie", "Elvina", "Elvira", "Elwanda", "Elyse", "Elza", "Ema", "Emelda", "Emelia", "Emelina",
"Emeline", "Emely", "Emerald", "Emerita", "Emiko", "Emilee", "Emilia", "Emilie", "Emily", "Emma", "Emmaline", "Emmie", "Emmy",
"Emogene", "Ena", "Enda", "Enedina", "Eneida", "Enid", "Enola", "Enriqueta", "Epifania", "Era", "Eric", "Erica", "Ericka",
"Erika", "Erin", "Erinn", "Erlene", "Erlinda", "Erline", "Erma", "Ermelinda", "Erminia", "Erna", "Ernestina", "Ernestine", "Eryn",
"Esmeralda", "Esperanza", "Essie", "Esta", "Estefana", "Estela", "Estell", "Estella", "Estelle", "Ester", "Esther", "Estrella",
"Etha", "Ethel", "Ethelene", "Ethelyn", "Ethyl", "Etsuko", "Etta", "Ettie", "Eufemia", "Eugena", "Eugene", "Eugenia", "Eugenie",
"Eula", "Eulah", "Eulalia", "Eun", "Euna", "Eunice", "Eura", "Eusebia", "Eustolia", "Eva", "Evalyn", "Evan", "Evangelina",
"Evangeline", "Eve", "Evelia", "Evelin", "Evelina", "Eveline", "Evelyn", "Evelyne", "Evelynn", "Evette", "Evia", "Evie", "Evita",
"Evon", "Evonne", "Ewa", "Exie", "Fabiola", "Fairy", "Faith", "Fallon", "Fannie", "Fanny", "Farah", "Farrah", "Fatima", "Fatimah",
"Faustina", "Faviola", "Fawn", "Fay", "Faye", "Fe", "Felecia", "Felica", "Felice", "Felicia", "Felicidad", "Felicita",
"Felicitas", "Felipa", "Felisa", "Felisha", "Fermina", "Fern", "Fernanda", "Fernande", "Ferne", "Fidela", "Fidelia", "Filomena",
"Fiona", "Flavia", "Fleta", "Flo", "Flor", "Flora", "Florance", "Florence", "Florencia", "Florene", "Florentina", "Floretta",
"Floria", "Florida", "Florinda", "Florine", "Florrie", "Flossie", "Floy", "Fonda", "Fran", "France", "Francene", "Frances",
"Francesca", "Franchesca", "Francie", "Francina", "Francine", "Francis", "Francisca", "Francisco", "Francoise", "Frank",
"Frankie", "Fransisca", "Fred", "Freda", "Fredda", "Freddie", "Frederica", "Fredericka", "Fredia", "Fredricka", "Freeda",
"Freida", "Frida", "Frieda", "Fumiko", "Gabriel", "Gabriela", "Gabriele", "Gabriella", "Gabrielle", "Gail", "Gala", "Gale",
"Galina", "Garnet", "Garnett", "Gary", "Gay", "Gaye", "Gayla", "Gayle", "Gaylene", "Gaynell", "Gaynelle", "Gearldine", "Gema",
"Gemma", "Gena", "Gene", "Genesis", "Geneva", "Genevie", "Genevieve", "Genevive", "Genia", "Genie", "Genna", "Gennie", "Genny",
"Genoveva", "Georgann", "George", "Georgeann", "Georgeanna", "Georgene", "Georgetta", "Georgette", "Georgia", "Georgiana",
"Georgiann", "Georgianna", "Georgianne", "Georgie", "Georgina", "Georgine", "Gerald", "Geraldine", "Geralyn", "Gerda", "Geri",
"Germaine", "Gerri", "Gerry", "Gertha", "Gertie", "Gertrud", "Gertrude", "Gertrudis", "Gertude", "Ghislaine", "Gia", "Gianna",
"Gidget", "Gigi", "Gilberte", "Gilda", "Gillian", "Gilma", "Gina", "Ginette", "Ginger", "Ginny", "Giovanna", "Gisela", "Gisele",
"Giselle", "Gita", "Giuseppina", "Gladis", "Glady", "Gladys", "Glayds", "Glenda", "Glendora", "Glenn", "Glenna", "Glennie",
"Glennis", "Glinda", "Gloria", "Glory", "Glynda", "Glynis", "Golda", "Golden", "Goldie", "Grace", "Gracia", "Gracie", "Graciela",
"Grayce", "Grazyna", "Gregoria", "Gregory", "Greta", "Gretchen", "Gretta", "Gricelda", "Grisel", "Griselda", "Guadalupe",
"Gudrun", "Guillermina", "Gussie", "Gwen", "Gwenda", "Gwendolyn", "Gwenn", "Gwyn", "Gwyneth", "Ha", "Hae", "Hailey", "Haley",
"Halina", "Halley", "Hallie", "Han", "Hana", "Hang", "Hanh", "Hanna", "Hannah", "Hannelore", "Harmony", "Harold", "Harriet",
"Harriett", "Harriette", "Hassie", "Hattie", "Haydee", "Hayley", "Hazel", "Heather", "Hedwig", "Hedy", "Hee", "Heide", "Heidi",
"Heidy", "Heike", "Helaine", "Helen", "Helena", "Helene", "Helga", "Hellen", "Henrietta", "Henriette", "Henry", "Herlinda",
"Herma", "Hermelinda", "Hermila", "Hermina", "Hermine", "Herminia", "Herta", "Hertha", "Hester", "Hettie", "Hiedi", "Hien",
"Hilaria", "Hilary", "Hilda", "Hilde", "Hildegard", "Hildegarde", "Hildred", "Hillary", "Hilma", "Hiroko", "Hisako", "Hoa",
"Holley", "Holli", "Hollie", "Hollis", "Holly", "Honey", "Hong", "Hope", "Hortencia", "Hortense", "Hortensia", "Hsiu", "Hui",
"Hulda", "Huong", "Hwa", "Hyacinth", "Hye", "Hyo", "Hyon", "Hyun", "Ida", "Idalia", "Idell", "Idella", "Iesha", "Ignacia", "Ila",
"Ilana", "Ilda", "Ileana", "Ileen", "Ilene", "Iliana", "Illa", "Ilona", "Ilse", "Iluminada", "Ima", "Imelda", "Imogene", "In",
"Ina", "India", "Indira", "Inell", "Ines", "Inez", "Inga", "Inge", "Ingeborg", "Inger", "Ingrid", "Inocencia", "Iola", "Iona",
"Ione", "Ira", "Iraida", "Irena", "Irene", "Irina", "Iris", "Irish", "Irma", "Irmgard", "Isa", "Isabel", "Isabell", "Isabella",
"Isabelle", "Isadora", "Isaura", "Isela", "Isidra", "Isis", "Isobel", "Iva", "Ivana", "Ivelisse", "Ivette", "Ivey", "Ivonne",
"Ivy", "Izetta", "Izola", "Ja", "Jacalyn", "Jacelyn", "Jacinda", "Jacinta", "Jack", "Jackeline", "Jackelyn", "Jacki", "Jackie",
"Jacklyn", "Jackqueline", "Jaclyn", "Jacqualine", "Jacque", "Jacquelin", "Jacqueline", "Jacquelyn", "Jacquelyne", "Jacquelynn",
"Jacquetta", "Jacqui", "Jacquie", "Jacquiline", "Jacquline", "Jacqulyn", "Jada", "Jade", "Jadwiga", "Jae", "Jaime", "Jaimee",
"Jaimie", "Jaleesa", "Jalisa", "Jama", "Jame", "Jamee", "James", "Jamey", "Jami", "Jamie", "Jamika", "Jamila", "Jammie", "Jan",
"Jana", "Janae", "Janay", "Jane", "Janean", "Janee", "Janeen", "Janel", "Janell", "Janella", "Janelle", "Janene", "Janessa",
"Janet", "Janeth", "Janett", "Janetta", "Janette", "Janey", "Jani", "Janice", "Janie", "Janiece", "Janina", "Janine", "Janis",
"Janise", "Janita", "Jann", "Janna", "Jannet", "Jannette", "Jannie", "January", "Janyce", "Jaqueline", "Jaquelyn", "Jasmin",
"Jasmine", "Jason", "Jaunita", "Jay", "Jaye", "Jayme", "Jaymie", "Jayne", "Jazmin", "Jazmine", "Jean", "Jeana", "Jeane",
"Jeanelle", "Jeanene", "Jeanett", "Jeanetta", "Jeanette", "Jeanice", "Jeanie", "Jeanine", "Jeanmarie", "Jeanna", "Jeanne",
"Jeannetta", "Jeannette", "Jeannie", "Jeannine", "Jeffie", "Jeffrey", "Jen", "Jena", "Jenae", "Jene", "Jenee", "Jenell",
"Jenelle", "Jenette", "Jeneva", "Jeni", "Jenice", "Jenifer", "Jeniffer", "Jenine", "Jenise", "Jenna", "Jennefer", "Jennell",
"Jennette", "Jenni", "Jennie", "Jennifer", "Jenniffer", "Jennine", "Jenny", "Jeraldine", "Jeremy", "Jeri", "Jerica", "Jerilyn",
"Jerlene", "Jerri", "Jerrica", "Jerrie", "Jerry", "Jesenia", "Jesica", "Jesse", "Jessenia", "Jessi", "Jessia", "Jessica",
"Jessie", "Jessika", "Jestine", "Jesus", "Jesusa", "Jesusita", "Jetta", "Jettie", "Jewel", "Jewell", "Ji", "Jill", "Jillian",
"Jimmie", "Jimmy", "Jin", "Jina", "Jinny", "Jo", "Joan", "Joana", "Joane", "Joanie", "Joann", "Joanna", "Joanne", "Joaquina",
"Jocelyn", "Jodee", "Jodi", "Jodie", "Jody", "Joe", "Joeann", "Joel", "Joella", "Joelle", "Joellen", "Joetta", "Joette", "Joey",
"Johana", "Johanna", "Johanne", "John", "Johna", "Johnetta", "Johnette", "Johnie", "Johnna", "Johnnie", "Johnny", "Johnsie",
"Joi", "Joie", "Jolanda", "Joleen", "Jolene", "Jolie", "Joline", "Jolyn", "Jolynn", "Jon", "Jona", "Jone", "Jonell", "Jonelle",
"Jong", "Joni", "Jonie", "Jonna", "Jonnie", "Jordan", "Jose", "Josefa", "Josefina", "Josefine", "Joselyn", "Joseph", "Josephina",
"Josephine", "Josette", "Joshua", "Josie", "Joslyn", "Josphine", "Jovan", "Jovita", "Joy", "Joya", "Joyce", "Joycelyn", "Joye",
"Juan", "Juana", "Juanita", "Jude", "Judi", "Judie", "Judith", "Judy", "Jule", "Julee", "Julene", "Juli", "Julia", "Julian",
"Juliana", "Juliane", "Juliann", "Julianna", "Julianne", "Julie", "Julieann", "Julienne", "Juliet", "Julieta", "Julietta",
"Juliette", "Julio", "Julissa", "June", "Jung", "Junie", "Junita", "Junko", "Justa", "Justin", "Justine", "Jutta", "Ka", "Kacey",
"Kaci", "Kacie", "Kacy", "Kai", "Kaila", "Kaitlin", "Kaitlyn", "Kala", "Kaleigh", "Kaley", "Kali", "Kallie", "Kalyn", "Kam",
"Kamala", "Kami", "Kamilah", "Kandace", "Kandi", "Kandice", "Kandis", "Kandra", "Kandy", "Kanesha", "Kanisha", "Kara", "Karan",
"Kareen", "Karen", "Karena", "Karey", "Kari", "Karima", "Karin", "Karina", "Karine", "Karisa", "Karissa", "Karl", "Karla",
"Karleen", "Karlene", "Karly", "Karlyn", "Karmen", "Karol", "Karole", "Karoline", "Karolyn", "Karon", "Karren", "Karri", "Karrie",
"Karry", "Kary", "Karyl", "Karyn", "Kasandra", "Kasey", "Kasha", "Kasi", "Kasie", "Kassandra", "Kassie", "Kate", "Katelin",
"Katelyn", "Katelynn", "Katerine", "Kathaleen", "Katharina", "Katharine", "Katharyn", "Kathe", "Katheleen", "Katherin",
"Katherina", "Katherine", "Kathern", "Katheryn", "Kathey", "Kathi", "Kathie", "Kathleen", "Kathlene", "Kathline", "Kathlyn",
"Kathrin", "Kathrine", "Kathryn", "Kathryne", "Kathy", "Kathyrn", "Kati", "Katia", "Katie", "Katina", "Katlyn", "Katrice",
"Katrina", "Kattie", "Katy", "Kay", "Kayce", "Kaycee", "Kaye", "Kayla", "Kaylee", "Kayleen", "Kayleigh", "Kaylene", "Kazuko",
"Kecia", "Keeley", "Keely", "Keena", "Keesha", "Keiko", "Keila", "Keira", "Keisha", "Keith", "Keitha", "Keli", "Kelle", "Kellee",
"Kelley", "Kelli", "Kellie", "Kelly", "Kellye", "Kelsey", "Kelsi", "Kelsie", "Kemberly", "Kena", "Kenda", "Kendal", "Kendall",
"Kendra", "Kenia", "Kenisha", "Kenna", "Kenneth", "Kenya", "Kenyatta", "Kenyetta", "Kera", "Keren", "Keri", "Kerri", "Kerrie",
"Kerry", "Kerstin", "Kesha", "Keshia", "Keturah", "Keva", "Kevin", "Khadijah", "Khalilah", "Kia", "Kiana", "Kiara", "Kiera",
"Kiersten", "Kiesha", "Kiley", "Kim", "Kimber", "Kimberely", "Kimberlee", "Kimberley", "Kimberli", "Kimberlie", "Kimberly",
"Kimbery", "Kimbra", "Kimi", "Kimiko", "Kina", "Kindra", "Kira", "Kirby", "Kirsten", "Kirstie", "Kirstin", "Kisha", "Kit",
"Kittie", "Kitty", "Kiyoko", "Kizzie", "Kizzy", "Klara", "Kori", "Kortney", "Kourtney", "Kris", "Krishna", "Krissy", "Krista",
"Kristal", "Kristan", "Kristeen", "Kristel", "Kristen", "Kristi", "Kristian", "Kristie", "Kristin", "Kristina", "Kristine",
"Kristle", "Kristy", "Kristyn", "Krysta", "Krystal", "Krysten", "Krystin", "Krystina", "Krystle", "Krystyna", "Kum", "Kyla",
"Kyle", "Kylee", "Kylie", "Kym", "Kymberly", "Kyoko", "Kyong", "Kyra", "Kyung", "Lacey", "Lachelle", "Laci", "Lacie", "Lacresha",
"Lacy", "Ladawn", "Ladonna", "Lady", "Lael", "Lahoma", "Lai", "Laila", "Laine", "Lajuana", "Lakeesha", "Lakeisha", "Lakendra",
"Lakenya", "Lakesha", "Lakeshia", "Lakia", "Lakiesha", "Lakisha", "Lakita", "Lala", "Lamonica", "Lan", "Lana", "Lane", "Lanell",
"Lanelle", "Lanette", "Lang", "Lani", "Lanie", "Lanita", "Lannie", "Lanora", "Laquanda", "Laquita", "Lara", "Larae", "Laraine",
"Laree", "Larhonda", "Larisa", "Larissa", "Larita", "Laronda", "Larraine", "Larry", "Larue", "Lasandra", "Lashanda", "Lashandra",
"Lashaun", "Lashaunda", "Lashawn", "Lashawna", "Lashawnda", "Lashay", "Lashell", "Lashon", "Lashonda", "Lashunda", "Lasonya",
"Latanya", "Latarsha", "Latasha", "Latashia", "Latesha", "Latia", "Laticia", "Latina", "Latisha", "Latonia", "Latonya", "Latoria",
"Latosha", "Latoya", "Latoyia", "Latrice", "Latricia", "Latrina", "Latrisha", "Launa", "Laura", "Lauralee", "Lauran", "Laure",
"Laureen", "Laurel", "Lauren", "Laurena", "Laurence", "Laurene", "Lauretta", "Laurette", "Lauri", "Laurice", "Laurie", "Laurinda",
"Laurine", "Lauryn", "Lavada", "Lavelle", "Lavenia", "Lavera", "Lavern", "Laverna", "Laverne", "Laveta", "Lavette", "Lavina",
"Lavon", "Lavona", "Lavonda", "Lavone", "Lavonia", "Lavonna", "Lavonne", "Lawana", "Lawanda", "Lawanna", "Lawrence", "Layla",
"Layne", "Le", "Lea", "Leah", "Lean", "Leana", "Leandra", "Leann", "Leanna", "Leanne", "Leanora", "Leatha", "Leatrice", "Lecia",
"Leda", "Lee", "Leeann", "Leeanna", "Leeanne", "Leena", "Leesa", "Leia", "Leida", "Leigh", "Leigha", "Leighann", "Leila",
"Leilani", "Leisa", "Leisha", "Lekisha", "Lela", "Lelah", "Lelia", "Lena", "Lenita", "Lenna", "Lennie", "Lenora", "Lenore", "Leo",
"Leola", "Leoma", "Leon", "Leona", "Leonarda", "Leone", "Leonia", "Leonida", "Leonie", "Leonila", "Leonor", "Leonora", "Leonore",
"Leontine", "Leora", "Leota", "Lera", "Lesa", "Lesha", "Lesia", "Leslee", "Lesley", "Lesli", "Leslie", "Lessie", "Lester", "Leta",
"Letha", "Leticia", "Letisha", "Letitia", "Lettie", "Letty", "Lewis", "Lexie", "Lezlie", "Li", "Lia", "Liana", "Liane", "Lianne",
"Libbie", "Libby", "Liberty", "Librada", "Lida", "Lidia", "Lien", "Lieselotte", "Ligia", "Lila", "Lili", "Lilia", "Lilian",
"Liliana", "Lilla", "Lilli", "Lillia", "Lilliam", "Lillian", "Lilliana", "Lillie", "Lilly", "Lily", "Lin", "Lina", "Linda",
"Lindsay", "Lindsey", "Lindsy", "Lindy", "Linette", "Ling", "Linh", "Linn", "Linnea", "Linnie", "Linsey", "Lisa", "Lisabeth",
"Lisandra", "Lisbeth", "Lise", "Lisette", "Lisha", "Lissa", "Lissette", "Lita", "Livia", "Liz", "Liza", "Lizabeth", "Lizbeth",
"Lizeth", "Lizette", "Lizzette", "Lizzie", "Loan", "Logan", "Loida", "Lois", "Loise", "Lola", "Lolita", "Loma", "Lona", "Londa",
"Loni", "Lonna", "Lonnie", "Lora", "Loraine", "Loralee", "Lore", "Lorean", "Loree", "Loreen", "Lorelei", "Loren", "Lorena",
"Lorene", "Lorenza", "Loreta", "Loretta", "Lorette", "Lori", "Loria", "Loriann", "Lorie", "Lorilee", "Lorina", "Lorine", "Loris",
"Lorita", "Lorna", "Lorraine", "Lorretta", "Lorri", "Lorriane", "Lorrie", "Lorrine", "Lory", "Lottie", "Lou", "Louann", "Louanne",
"Louella", "Louetta", "Louie", "Louis", "Louisa", "Louise", "Loura", "Lourdes", "Lourie", "Louvenia", "Love", "Lovella",
"Lovetta", "Lovie", "Loyce", "Lu", "Luana", "Luann", "Luanna", "Luanne", "Luba", "Luci", "Lucia", "Luciana", "Lucie", "Lucienne",
"Lucila", "Lucile", "Lucilla", "Lucille", "Lucina", "Lucinda", "Lucrecia", "Lucretia", "Lucy", "Ludie", "Ludivina", "Lue",
"Luella", "Luetta", "Luis", "Luisa", "Luise", "Lula", "Lulu", "Luna", "Lupe", "Lupita", "Lura", "Lurlene", "Lurline", "Luvenia",
"Luz", "Lyda", "Lydia", "Lyla", "Lyn", "Lynda", "Lyndia", "Lyndsay", "Lyndsey", "Lynell", "Lynelle", "Lynetta", "Lynette", "Lynn",
"Lynna", "Lynne", "Lynnette", "Lynsey", "Ma", "Mabel", "Mabelle", "Mable", "Machelle", "Macie", "Mackenzie", "Macy", "Madalene",
"Madaline", "Madalyn", "Maddie", "Madelaine", "Madeleine", "Madelene", "Madeline", "Madelyn", "Madge", "Madie", "Madison",
"Madlyn", "Madonna", "Mae", "Maegan", "Mafalda", "Magali", "Magaly", "Magan", "Magaret", "Magda", "Magdalen", "Magdalena",
"Magdalene", "Magen", "Maggie", "Magnolia", "Mahalia", "Mai", "Maia", "Maida", "Maile", "Maira", "Maire", "Maisha", "Maisie",
"Majorie", "Makeda", "Malena", "Malia", "Malika", "Malinda", "Malisa", "Malissa", "Malka", "Mallie", "Mallory", "Malorie",
"Malvina", "Mamie", "Mammie", "Man", "Mana", "Manda", "Mandi", "Mandie", "Mandy", "Manie", "Manuela", "Many", "Mao", "Maple",
"Mara", "Maragaret", "Maragret", "Maranda", "Marcela", "Marcelene", "Marcelina", "Marceline", "Marcell", "Marcella", "Marcelle",
"Marcene", "Marchelle", "Marci", "Marcia", "Marcie", "Marcy", "Mardell", "Maren", "Marg", "Margaret", "Margareta", "Margarete",
"Margarett", "Margaretta", "Margarette", "Margarita", "Margarite", "Margart", "Marge", "Margene", "Margeret", "Margert",
"Margery", "Marget", "Margherita", "Margie", "Margit", "Margo", "Margorie", "Margot", "Margret", "Margrett", "Marguerita",
"Marguerite", "Margurite", "Margy", "Marhta", "Mari", "Maria", "Mariah", "Mariam", "Marian", "Mariana", "Marianela", "Mariann",
"Marianna", "Marianne", "Maribel", "Maribeth", "Marica", "Maricela", "Maricruz", "Marie", "Mariel", "Mariela", "Mariella",
"Marielle", "Marietta", "Mariette", "Mariko", "Marilee", "Marilou", "Marilu", "Marilyn", "Marilynn", "Marin", "Marina", "Marinda",
"Marine", "Mario", "Marion", "Maris", "Marisa", "Marisela", "Marisha", "Marisol", "Marissa", "Marita", "Maritza", "Marivel",
"Marjorie", "Marjory", "Marketta", "Markita", "Marla", "Marlana", "Marleen", "Marlen", "Marlena", "Marlene", "Marlin", "Marline",
"Marlo", "Marlyn", "Marlys", "Marna", "Marni", "Marnie", "Marquerite", "Marquetta", "Marquita", "Marquitta", "Marry", "Marsha",
"Marshall", "Marta", "Marth", "Martha", "Marti", "Martin", "Martina", "Martine", "Marty", "Marva", "Marvel", "Marvella", "Marvis",
"Marx", "Mary", "Marya", "Maryalice", "Maryam", "Maryann", "Maryanna", "Maryanne", "Marybelle", "Marybeth", "Maryellen",
"Maryetta", "Maryjane", "Maryjo", "Maryland", "Marylee", "Marylin", "Maryln", "Marylou", "Marylouise", "Marylyn", "Marylynn",
"Maryrose", "Masako", "Matha", "Mathilda", "Mathilde", "Matilda", "Matilde", "Matthew", "Mattie", "Maud", "Maude", "Maudie",
"Maura", "Maureen", "Maurice", "Maurine", "Maurita", "Mavis", "Maxie", "Maxima", "Maximina", "Maxine", "May", "Maya", "Maybell",
"Maybelle", "Maye", "Mayme", "Mayola", "Mayra", "Mazie", "Mckenzie", "Meagan", "Meaghan", "Mechelle", "Meda", "Mee", "Meg",
"Megan", "Meggan", "Meghan", "Meghann", "Mei", "Melaine", "Melani", "Melania", "Melanie", "Melany", "Melba", "Melda", "Melia",
"Melida", "Melina", "Melinda", "Melisa", "Melissa", "Melissia", "Melita", "Mellie", "Mellisa", "Mellissa", "Melodee", "Melodi",
"Melodie", "Melody", "Melonie", "Melony", "Melva", "Melvin", "Melvina", "Melynda", "Mendy", "Mercedes", "Mercedez", "Mercy",
"Meredith", "Meri", "Merideth", "Meridith", "Merilyn", "Merissa", "Merle", "Merlene", "Merlyn", "Merna", "Merri", "Merrie",
"Merrilee", "Merry", "Mertie", "Meryl", "Meta", "Mi", "Mia", "Mica", "Micaela", "Micah", "Micha", "Michael", "Michaela",
"Michaele", "Michal", "Micheal", "Michel", "Michele", "Michelina", "Micheline", "Michell", "Michelle", "Michiko", "Mickey",
"Micki", "Mickie", "Miesha", "Migdalia", "Mignon", "Miguelina", "Mika", "Mikaela", "Mike", "Miki", "Mikki", "Mila", "Milagro",
"Milagros", "Milda", "Mildred", "Milissa", "Millicent", "Millie", "Milly", "Mimi", "Min", "Mina", "Minda", "Mindi", "Mindy",
"Minerva", "Ming", "Minh", "Minna", "Minnie", "Minta", "Mira", "Miranda", "Mireille", "Mirella", "Mireya", "Miriam", "Mirian",
"Mirna", "Mirta", "Mirtha", "Misha", "Miss", "Missy", "Misti", "Mistie", "Misty", "Mitchell", "Mitsue", "Mitsuko", "Mittie",
"Mitzi", "Mitzie", "Miyoko", "Modesta", "Moira", "Mollie", "Molly", "Mona", "Monet", "Monica", "Monika", "Monique", "Monnie",
"Monserrate", "Moon", "Mora", "Morgan", "Moriah", "Mozell", "Mozella", "Mozelle", "Mui", "Muoi", "Muriel", "My", "Myesha",
"Myong", "Myra", "Myriam", "Myrl", "Myrle", "Myrna", "Myrta", "Myrtice", "Myrtie", "Myrtis", "Myrtle", "Myung", "Na", "Nada",
"Nadene", "Nadia", "Nadine", "Naida", "Nakesha", "Nakia", "Nakisha", "Nakita", "Nam", "Nan", "Nana", "Nancee", "Nancey", "Nanci",
"Nancie", "Nancy", "Nanette", "Nannette", "Nannie", "Naoma", "Naomi", "Narcisa", "Natacha", "Natalia", "Natalie", "Natalya",
"Natasha", "Natashia", "Nathalie", "Natisha", "Natividad", "Natosha", "Necole", "Nedra", "Neely", "Neida", "Nelda", "Nelia",
"Nelida", "Nell", "Nella", "Nelle", "Nellie", "Nelly", "Nena", "Nenita", "Neoma", "Neomi", "Nereida", "Nerissa", "Nery", "Neta",
"Nettie", "Neva", "Nevada", "Nga", "Ngoc", "Nguyet", "Nia", "Nichelle", "Nichol", "Nichole", "Nicholle", "Nicki", "Nickie",
"Nickole", "Nicky", "Nicol", "Nicola", "Nicolasa", "Nicole", "Nicolette", "Nicolle", "Nida", "Nidia", "Niesha", "Nieves", "Niki",
"Nikia", "Nikita", "Nikki", "Nikole", "Nila", "Nilda", "Nilsa", "Nina", "Ninfa", "Nisha", "Nita", "Nobuko", "Noel", "Noelia",
"Noella", "Noelle", "Noemi", "Nohemi", "Nola", "Noma", "Nona", "Nora", "Norah", "Noreen", "Norene", "Noriko", "Norine", "Norma",
"Norman", "Nova", "Novella", "Nu", "Nubia", "Numbers", "Nydia", "Nyla", "Obdulia", "Ocie", "Octavia", "Oda", "Odelia", "Odell",
"Odessa", "Odette", "Odilia", "Ofelia", "Ok", "Ola", "Olene", "Oleta", "Olevia", "Olga", "Olimpia", "Olinda", "Oliva", "Olive",
"Olivia", "Ollie", "Olympia", "Oma", "Omega", "Ona", "Oneida", "Onie", "Onita", "Opal", "Ophelia", "Ora", "Oralee", "Oralia",
"Oretha", "Orpha", "Oscar", "Ossie", "Otelia", "Otha", "Otilia", "Ouida", "Ozell", "Ozella", "Ozie", "Pa", "Page", "Paige",
"Palma", "Palmira", "Pam", "Pamala", "Pamela", "Pamelia", "Pamella", "Pamila", "Pamula", "Pandora", "Pansy", "Paola", "Paris",
"Parthenia", "Particia", "Pasty", "Pat", "Patience", "Patria", "Patrica", "Patrice", "Patricia", "Patrick", "Patrina", "Patsy",
"Patti", "Pattie", "Patty", "Paul", "Paula", "Paulene", "Pauletta", "Paulette", "Paulina", "Pauline", "Paulita", "Paz", "Pearl",
"Pearle", "Pearlene", "Pearlie", "Pearline", "Pearly", "Peg", "Peggie", "Peggy", "Pei", "Penelope", "Penney", "Penni", "Pennie",
"Penny", "Perla", "Perry", "Peter", "Petra", "Petrina", "Petronila", "Phebe", "Phillis", "Philomena", "Phoebe", "Phung", "Phuong",
"Phylicia", "Phylis", "Phyliss", "Phyllis", "Pia", "Piedad", "Pilar", "Ping", "Pinkie", "Piper", "Pok", "Polly", "Porsche",
"Porsha", "Portia", "Precious", "Pricilla", "Princess", "Priscila", "Priscilla", "Providencia", "Prudence", "Pura", "Qiana",
"Queen", "Queenie", "Quiana", "Quinn", "Quyen", "Rachael", "Rachal", "Racheal", "Rachel", "Rachele", "Rachell", "Rachelle",
"Racquel", "Rae", "Raeann", "Raelene", "Rafaela", "Raguel", "Raina", "Raisa", "Ramona", "Ramonita", "Rana", "Ranae", "Randa",
"Randee", "Randi", "Randy", "Ranee", "Raquel", "Rasheeda", "Rashida", "Raven", "Ray", "Raye", "Raylene", "Raymond", "Raymonde",
"Rayna", "Rea", "Reagan", "Reanna", "Reatha", "Reba", "Rebbeca", "Rebbecca", "Rebeca", "Rebecca", "Rebecka", "Rebekah", "Reda",
"Reena", "Refugia", "Refugio", "Regan", "Regena", "Regenia", "Regina", "Regine", "Reginia", "Reiko", "Reina", "Reita", "Rema",
"Remedios", "Remona", "Rena", "Renae", "Renata", "Renate", "Renay", "Renda", "Rene", "Renea", "Renee", "Renetta", "Renita",
"Renna", "Ressie", "Reta", "Retha", "Retta", "Reva", "Reyna", "Reynalda", "Rhea", "Rheba", "Rhiannon", "Rhoda", "Rhona", "Rhonda",
"Ria", "Ricarda", "Richard", "Richelle", "Ricki", "Rickie", "Rikki", "Rima", "Rina", "Risa", "Rita", "Riva", "Rivka", "Robbi",
"Robbie", "Robbin", "Robbyn", "Robena", "Robert", "Roberta", "Roberto", "Robin", "Robyn", "Rochel", "Rochell", "Rochelle",
"Rocio", "Rolanda", "Rolande", "Roma", "Romaine", "Romana", "Romelia", "Romona", "Rona", "Ronald", "Ronda", "Roni", "Ronna",
"Ronni", "Ronnie", "Rory", "Rosa", "Rosalba", "Rosalee", "Rosalia", "Rosalie", "Rosalina", "Rosalind", "Rosalinda", "Rosaline",
"Rosalva", "Rosalyn", "Rosamaria", "Rosamond", "Rosana", "Rosann", "Rosanna", "Rosanne", "Rosaria", "Rosario", "Rosaura", "Rose",
"Roseann", "Roseanna", "Roseanne", "Roselee", "Roselia", "Roseline", "Rosella", "Roselle", "Roselyn", "Rosemarie", "Rosemary",
"Rosena", "Rosenda", "Rosetta", "Rosette", "Rosia", "Rosie", "Rosina", "Rosio", "Rosita", "Roslyn", "Rossana", "Rossie", "Rosy",
"Rowena", "Roxana", "Roxane", "Roxann", "Roxanna", "Roxanne", "Roxie", "Roxy", "Roy", "Royce", "Rozanne", "Rozella", "Rubi",
"Rubie", "Ruby", "Rubye", "Rudy", "Rufina", "Russell", "Ruth", "Rutha", "Ruthann", "Ruthanne", "Ruthe", "Ruthie", "Ryan", "Ryann",
"Sabina", "Sabine", "Sabra", "Sabrina", "Sacha", "Sachiko", "Sade", "Sadie", "Sadye", "Sage", "Salena", "Salina", "Salley",
"Sallie", "Sally", "Salome", "Sam", "Samantha", "Samara", "Samatha", "Samella", "Samira", "Sammie", "Sammy", "Samuel", "Sana",
"Sanda", "Sandee", "Sandi", "Sandie", "Sandra", "Sandy", "Sang", "Sanjuana", "Sanjuanita", "Sanora", "Santa", "Santana",
"Santina", "Santos", "Sara", "Sarah", "Sarai", "Saran", "Sari", "Sarina", "Sasha", "Saturnina", "Sau", "Saundra", "Savanna",
"Savannah", "Scarlet", "Scarlett", "Scott", "Scottie", "Sean", "Season", "Sebrina", "See", "Seema", "Selena", "Selene", "Selina",
"Selma", "Sena", "Senaida", "September", "Serafina", "Serena", "Serina", "Serita", "Setsuko", "Sha", "Shae", "Shaina", "Shakia",
"Shakira", "Shakita", "Shala", "Shalanda", "Shalon", "Shalonda", "Shameka", "Shamika", "Shan", "Shana", "Shanae", "Shanda",
"Shandi", "Shandra", "Shane", "Shaneka", "Shanel", "Shanell", "Shanelle", "Shani", "Shanice", "Shanika", "Shaniqua", "Shanita",
"Shanna", "Shannan", "Shannon", "Shanon", "Shanta", "Shantae", "Shantay", "Shante", "Shantel", "Shantell", "Shantelle", "Shanti",
"Shaquana", "Shaquita", "Shara", "Sharan", "Sharda", "Sharee", "Sharell", "Sharen", "Shari", "Sharice", "Sharie", "Sharika",
"Sharilyn", "Sharita", "Sharla", "Sharleen", "Sharlene", "Sharmaine", "Sharolyn", "Sharon", "Sharonda", "Sharri", "Sharron",
"Sharyl", "Sharyn", "Shasta", "Shaun", "Shauna", "Shaunda", "Shaunna", "Shaunta", "Shaunte", "Shavon", "Shavonda", "Shavonne",
"Shawana", "Shawanda", "Shawanna", "Shawn", "Shawna", "Shawnda", "Shawnee", "Shawnna", "Shawnta", "Shay", "Shayla", "Shayna",
"Shayne", "Shea", "Sheba", "Sheena", "Sheila", "Sheilah", "Shela", "Shelba", "Shelby", "Shelia", "Shella", "Shelley", "Shelli",
"Shellie", "Shelly", "Shemeka", "Shena", "Shenika", "Shenita", "Shenna", "Shera", "Sheree", "Sherell", "Sheri", "Sherice",
"Sheridan", "Sherie", "Sherika", "Sherill", "Sherilyn", "Sherise", "Sherita", "Sherlene", "Sherley", "Sherly", "Sherlyn",
"Sheron", "Sherrell", "Sherri", "Sherrie", "Sherril", "Sherrill", "Sherron", "Sherry", "Sherryl", "Shery", "Sheryl", "Sheryll",
"Shiela", "Shila", "Shiloh", "Shin", "Shira", "Shirely", "Shirl", "Shirlee", "Shirleen", "Shirlene", "Shirley", "Shirly",
"Shizue", "Shizuko", "Shona", "Shonda", "Shondra", "Shonna", "Shonta", "Shoshana", "Shu", "Shyla", "Sibyl", "Sidney", "Sierra",
"Signe", "Sigrid", "Silva", "Silvana", "Silvia", "Sima", "Simona", "Simone", "Simonne", "Sina", "Sindy", "Siobhan", "Sirena",
"Siu", "Sixta", "Skye", "Slyvia", "So", "Socorro", "Sofia", "Soila", "Sol", "Solange", "Soledad", "Somer", "Sommer", "Son",
"Sona", "Sondra", "Song", "Sonia", "Sonja", "Sonya", "Soo", "Sook", "Soon", "Sophia", "Sophie", "Soraya", "Sparkle", "Spring",
"Stacey", "Staci", "Stacia", "Stacie", "Stacy", "Starla", "Starr", "Stasia", "Stefani", "Stefania", "Stefanie", "Stefany",
"Steffanie", "Stella", "Stepanie", "Stephaine", "Stephane", "Stephani", "Stephania", "Stephanie", "Stephany", "Stephen",
"Stephenie", "Stephine", "Stephnie", "Steven", "Stevie", "Stormy", "Su", "Suanne", "Sudie", "Sue", "Sueann", "Suellen", "Suk",
"Sulema", "Sumiko", "Summer", "Sun", "Sunday", "Sung", "Sunni", "Sunny", "Sunshine", "Susan", "Susana", "Susann", "Susanna",
"Susannah", "Susanne", "Susie", "Susy", "Suzan", "Suzann", "Suzanna", "Suzanne", "Suzette", "Suzi", "Suzie", "Suzy", "Svetlana",
"Sybil", "Syble", "Sydney", "Sylvia", "Sylvie", "Synthia", "Syreeta", "Ta", "Tabatha", "Tabetha", "Tabitha", "Tai", "Taina",
"Taisha", "Tajuana", "Takako", "Takisha", "Talia", "Talisha", "Talitha", "Tam", "Tama", "Tamala", "Tamar", "Tamara", "Tamatha",
"Tambra", "Tameika", "Tameka", "Tamekia", "Tamela", "Tamera", "Tamesha", "Tami", "Tamica", "Tamie", "Tamika", "Tamiko", "Tamisha",
"Tammara", "Tammera", "Tammi", "Tammie", "Tammy", "Tamra", "Tana", "Tandra", "Tandy", "Taneka", "Tanesha", "Tangela", "Tania",
"Tanika", "Tanisha", "Tanja", "Tanna", "Tanya", "Tara", "Tarah", "Taren", "Tari", "Tarra", "Tarsha", "Taryn", "Tasha", "Tashia",
"Tashina", "Tasia", "Tatiana", "Tatum", "Tatyana", "Taunya", "Tawana", "Tawanda", "Tawanna", "Tawna", "Tawny", "Tawnya", "Taylor",
"Tayna", "Teena", "Tegan", "Teisha", "Telma", "Temeka", "Temika", "Tempie", "Temple", "Tena", "Tenesha", "Tenisha", "Tennie",
"Tennille", "Teodora", "Teofila", "Tequila", "Tera", "Tereasa", "Teresa", "Terese", "Teresia", "Teresita", "Teressa", "Teri",
"Terica", "Terina", "Terisa", "Terra", "Terrell", "Terresa", "Terri", "Terrie", "Terrilyn", "Terry", "Tesha", "Tess", "Tessa",
"Tessie", "Thalia", "Thanh", "Thao", "Thea", "Theda", "Thelma", "Theo", "Theodora", "Theola", "Theresa", "Therese", "Theresia",
"Theressa", "Thersa", "Thi", "Thomas", "Thomasena", "Thomasina", "Thomasine", "Thora", "Thresa", "Thu", "Thuy", "Tia", "Tiana",
"Tianna", "Tiara", "Tien", "Tiera", "Tierra", "Tiesha", "Tifany", "Tiffaney", "Tiffani", "Tiffanie", "Tiffany", "Tiffiny",
"Tijuana", "Tilda", "Tillie", "Timika", "Timothy", "Tina", "Tinisha", "Tiny", "Tisa", "Tish", "Tisha", "Tobi", "Tobie", "Toby",
"Toccara", "Toi", "Tomasa", "Tomeka", "Tomi", "Tomika", "Tomiko", "Tommie", "Tommy", "Tommye", "Tomoko", "Tona", "Tonda",
"Tonette", "Toni", "Tonia", "Tonie", "Tonisha", "Tonita", "Tonja", "Tony", "Tonya", "Tora", "Tori", "Torie", "Torri", "Torrie",
"Tory", "Tosha", "Toshia", "Toshiko", "Tova", "Towanda", "Toya", "Tracee", "Tracey", "Traci", "Tracie", "Tracy", "Tran", "Trang",
"Treasa", "Treena", "Trena", "Tresa", "Tressa", "Tressie", "Treva", "Tricia", "Trina", "Trinh", "Trinidad", "Trinity", "Trish",
"Trisha", "Trista", "Tristan", "Troy", "Trudi", "Trudie", "Trudy", "Trula", "Tu", "Tula", "Tuyet", "Twana", "Twanna", "Twila",
"Twyla", "Tyesha", "Tyisha", "Tyler", "Tynisha", "Tyra", "Ula", "Ulrike", "Un", "Una", "Ursula", "Usha", "Ute", "Vada", "Val",
"Valarie", "Valda", "Valencia", "Valene", "Valentina", "Valentine", "Valeri", "Valeria", "Valerie", "Valery", "Vallie", "Valorie",
"Valrie", "Van", "Vanda", "Vanesa", "Vanessa", "Vanetta", "Vania", "Vanita", "Vanna", "Vannesa", "Vannessa", "Vashti", "Vasiliki",
"Veda", "Velda", "Velia", "Vella", "Velma", "Velva", "Velvet", "Vena", "Venessa", "Venetta", "Venice", "Venita", "Vennie",
"Venus", "Veola", "Vera", "Verda", "Verdell", "Verdie", "Verena", "Vergie", "Verla", "Verlene", "Verlie", "Verline", "Verna",
"Vernell", "Vernetta", "Vernia", "Vernice", "Vernie", "Vernita", "Vernon", "Verona", "Veronica", "Veronique", "Versie", "Vertie",
"Vesta", "Veta", "Vi", "Vicenta", "Vickey", "Vicki", "Vicky", "Victor", "Victoria", "Victorina", "Vida", "Viki", "Vikki", "Vilma",
"Vina", "Vincenza", "Vinita", "Vinnie", "Viola", "Violet", "Violeta", "Violette", "Virgen", "Virgie", "Virgil", "Virgina",
"Virginia", "Vita", "Viva", "Vivan", "Vivian", "Viviana", "Vivien", "Vivienne", "Voncile", "Vonda", "Vonnie", "Wai", "Waltraud",
"Wan", "Wanda", "Waneta", "Wanetta", "Wanita", "Wava", "Wei", "Wen", "Wendi", "Wendie", "Wendolyn", "Wendy", "Wenona", "Wesley",
"Whitley", "Whitney", "Wilda", "Wilhelmina", "Wilhemina", "Willa", "Willena", "Willene", "Willetta", "Willette", "Willia",
"William", "Willie", "Williemae", "Willodean", "Willow", "Wilma", "Windy", "Winifred", "Winnie", "Winnifred", "Winona", "Winter",
"Wonda", "Wynell", "Wynona", "Xenia", "Xiao", "Xiomara", "Xochitl", "Xuan", "Yadira", "Yaeko", "Yael", "Yahaira", "Yajaira",
"Yan", "Yang", "Yanira", "Yasmin", "Yasmine", "Yasuko", "Yee", "Yelena", "Yen", "Yer", "Yesenia", "Yessenia", "Yetta", "Yevette",
"Yi", "Ying", "Yoko", "Yolanda", "Yolande", "Yolando", "Yolonda", "Yon", "Yong", "Yoshie", "Yoshiko", "Youlanda", "Young", "Yu",
"Yuette", "Yuk", "Yuki", "Yukiko", "Yuko", "Yulanda", "Yun", "Yung", "Yuonne", "Yuri", "Yuriko", "Yvette", "Yvone", "Yvonne",
"Zada", "Zaida", "Zana", "Zandra", "Zelda", "Zella", "Zelma", "Zena", "Zenaida", "Zenia", "Zenobia", "Zetta", "Zina", "Zita",
"Zoe", "Zofia", "Zoila", "Zola", "Zona", "Zonia", "Zora", "Zoraida", "Zula", "Zulema", "Zulma", "Agueda", "Alysa", "Elease",
"Elmira", "Fae", "Hue", "Ivory", "Jayna", "Joannie", "Justina", "Karie", "Karma", "Lavinia", "Lorinda", "Mark", "Merrill", "Neda",
"Ngan", "Sarita", "Shemika", "Stacee", "Star", "Travis", "Twanda", "Veronika", "Vickie"
};

const char *lastnames[] = {
        "Cohen", "Maddox", "Blevins", "Mercer", "Turow", "Mercado", "Blanchard", "Valentine", "Hale", "Dillard", "White", "Blackburn", "Pennington", "Wyatt", "Blair", "Hurst", "Bartlett", "Noel", "Puckett", "Spalding", "Barlow", "Aceveds",
        "Diamond", "Knox", "Vinson", "Abbott", "Bishop", "Mullen", "Lancaster", "Holcomb", "Aaron", "Abbott", "Ackerman", "Aldaine", "Alvarez", "Anders", "Babcock", "Bachelor", "Bagwell", "Bailey", "Cameron", "Camp", "Canterbury", "Carey",
        "Carlisle", "Cruso", "Daily", "Dacanay", "Dean", "Dreygon", "East", "Eaton", "England", "Espinoza", "Evans", "Farmer", "Farris", "Faulkner", "Feliciano", "Ferguson", "Gall", "Gallegos", "Galvan", "Gamble", "Garcia", "Gretzky",
        "Hackett", "Haddock", "Haines", "Hernandez", "Ingalls", "Inman", "Iver", "Jaquez", "Jarrett", "Jarvis", "Keefe", "Keifer", "Kendall", "Larson", "Latimer", "Laughlin", "Lawrence", "Maestas", "Magnuson", "Majors", "Nixon", "Noble",
        "North", "O'Toole", "Osborne", "Oswald", "Perez", "Petty", "Pevensey", "Paasch", "Pfeiffer", "Phelps", "Polanski", "Quaid", "Quaker", "Radford", "Radner", "Ralston", "Rodrigues", "Rabenstein", "Rosati", "Sadler", "Sanchez", "Searle",
        "Sedgwick", "Taheny", "Thatcher", "Torres", "Townsend", "Tredway", "Tremaine", "Urban", "Usher", "Van Zandt", "Vladamire", "Webster", "Weinstein", "Yeager", "Yeats", "Young", "Zedler", "Zimmerman", "Zuniga", "Zabinski"
};

#define MALE_GIVEN_NAMES   1217
#define FEMALE_GIVEN_NAMES 4273
#define LASTNAMES 125

NPC::NPC()
{
        TCODColor fg, bg;

        setstat(sHealth, 100);
        setchar('@');

        fg = display->get_random_color();
        bg = TCODColor::black; // display->get_random_color();
        setcolors(fg, bg);
        setai(AI_RANDOM);
        path = new TCODPath(world->a->tcodmap, 1.0f);
        has_goal = false;
        setgender(ri(0, 1));
        generate_name();
}

NPC::~NPC()
{
        delete path;
}

void NPC::generate_name()
{
        char n[64];
        if(is_male()) {
                int i = ri(0, MALE_GIVEN_NAMES);
                strcpy(n, malegivenname[i]);
        } else {
                int i = ri(0, FEMALE_GIVEN_NAMES);
                strcpy(n, femalegivenname[i]);
        }
        strcat(n, " ");
        int i = ri(0, LASTNAMES);
        strcat(n, lastnames[i]);

        setname(n);
}

void NPC::setai(int which)
{
        which_ai = which;
}

void NPC::random_ai()
{
        int choice = ri(1,9);
        switch (choice) {
                case 1:
                        this->move_sw();
                        break;
                case 2:
                        this->move_down();
                        break;
                case 3:
                        this->move_se();
                        break;
                case 4:
                        this->move_left();
                        break;
                case 5:
                        break;
                case 6:
                        this->move_right();
                        break;
                case 7:
                        this->move_nw();
                        break;
                case 8:
                        this->move_up();
                        break;
                case 9:
                        this->move_ne();
                        break;
                default:
                        break;
        }
}

void NPC::set_random_goal()
{
        int type;

        type = ri(1,4);
        if(type == 1) {    // goal is random location on the level.
                set_goal(world->get_random_walkable_cell(this->area_id));
        } else if(type == 2) {
                set_goal(world->a->stairs_up);
        } else if(type == 3) {
                set_goal(world->a->stairs_down);
        } else if(type == 4) {
                set_goal(world->get_random_walkable_cell(this->area_id));
        }
}

void NPC::path_ai()
{
        if(!has_goal) {
                set_random_goal();
        } else {
                // Let's walk the path!
                int x, y;
                coord_t c = this->getxy();
                path->compute(c.x, c.y, goal.x, goal.y);
                if(path->walk(&x, &y, true)) {
                        // success
                        if(x > c.x && y == c.y)
                                move_right();
                        if(x < c.x && y == c.y)
                                move_left();
                        if(x == c.x && y > c.y)
                                move_down();
                        if(x == c.x && y < c.y)
                                move_up();
                        if(x > c.x && y > c.y)
                                move_se();
                        if(x < c.x && y > c.y)
                                move_sw();
                        if(x < c.x && y < c.y)
                                move_nw();
                        if(x > c.x && y < c.y)
                                move_ne();
                } else {
                        // walking the path failed - set new goal.
                        has_goal = false;
                }
        }
}

void NPC::ai()
{
        switch(which_ai) {
                case AI_RANDOM:
                        this->random_ai();
                        break;
                case AI_PATH:
                        this->path_ai();
                        break;
                default:
                        dbg("no ai defined.");
                        break;
        }
}



// vim: fdm=syntax guifont=Terminus\ 8

