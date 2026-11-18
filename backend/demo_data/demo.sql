--
-- PostgreSQL database dump
--

\restrict cTV3y4pDbzL4EPZaCxKRrnNqo7vfOu40UGMdx9cNXy6JOpFHuFocZboYozyJNIT

-- Dumped from database version 17.11 (Debian 17.11-1.pgdg13+2)
-- Dumped by pg_dump version 17.11 (Debian 17.11-1.pgdg13+2)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET transaction_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Data for Name: users; Type: TABLE DATA; Schema: foodgram; Owner: -
--

INSERT INTO foodgram.users (id, password, last_login, username, first_name, last_name, email, date_joined, avatar) VALUES (1, 'sha256$ieSYS1DHx4LO2N4LXzkxzNx7yVHEcMTx$6f5a832e7682be66d1e462998135840f429baf431739439d1ed4ceeb974b9216', NULL, 'acenes', 'Денис', 'Жилин', 'den.jilin2017@yandex.ru', '2026-11-16 12:06:33.310352+00', NULL);
INSERT INTO foodgram.users (id, password, last_login, username, first_name, last_name, email, date_joined, avatar) VALUES (2, 'sha256$x7A8EPL9exso8GcvFt1skzRVZmDfOrgH$0bb3ca682dcbd1867e950da19f4432057db55d396d38f3c87d5a713ac063aa9e', NULL, 'mlavrov', 'Максим', 'Лавров', 'mlavrov@yandex.ru', '2026-11-16 12:53:31.005033+00', 'http://localhost/media/avatars/2.png');
INSERT INTO foodgram.users (id, password, last_login, username, first_name, last_name, email, date_joined, avatar) VALUES (3, 'sha256$fI6ybBXGTlxboVMQgzQbZXdQcvlBtSg6$58a99b4be2e782386b8e193e2a728a42bd5ad18f3116c369fdb23bb4adbb2301', NULL, 'vbarinov', 'Виктор', 'Баринов', 'vbarinov@yandex.ru', '2026-11-16 13:08:50.910568+00', 'http://localhost/media/avatars/3.jpg');
INSERT INTO foodgram.users (id, password, last_login, username, first_name, last_name, email, date_joined, avatar) VALUES (4, 'sha256$ehd2yBhOyMTa4FsbXutE7octPn7JvGUm$4c5a90706b9b821811788847124a6fad793aa16d0cd390efd4d5b24f438647f5', NULL, 'achuganin', 'Арсений', 'Чуганин', 'achuganin@yandex.ru', '2026-11-16 13:23:42.95457+00', 'http://localhost/media/avatars/4.jpg');


--
-- Data for Name: recipes; Type: TABLE DATA; Schema: foodgram; Owner: -
--

INSERT INTO foodgram.recipes (id, author_id, name, image, text, cooking_time) VALUES (1, 1, 'Пицца Пепперони', 'http://localhost/media/recipes/658e757c71cd471b84c260eb3e1a0726.jpg', 'Разогрейте духовку до 250°C. Раскатайте тесто в круг диаметром 30 см. Смажьте основу томатным соусом, отступив 1–2 см от края. Посыпьте половиной тёртой моцареллы. Выложите ломтики пепперони, затем посыпьте оставшейся моцареллой и 1 ч. л. сушёного орегано. Сбрызните 1 ст. л. оливкового масла. Выпекайте 10–12 минут до золотистого края. Дайте пицце остыть 2–3 минуты, нарежьте и подавайте.', 30);
INSERT INTO foodgram.recipes (id, author_id, name, image, text, cooking_time) VALUES (2, 1, 'Плов с говядиной', 'http://localhost/media/recipes/018d2086496441ee9da0d3b4402a49e5.jpg', 'Промойте 300 г риса до прозрачной воды. Говядину нарежьте кубиками 2–3 см, лук — кубиками, морковь — соломкой. В казане разогрейте 100 мл растительного масла, обжарьте 500 г мяса на сильном огне 10 минут, добавьте 200 г лука и обжаривайте 5 минут, затем добавьте 300 г моркови и обжаривайте ещё 7 минут. Добавьте 12 г соли, 3 г зиры, 5 г барбариса и 1,5 г чёрного перца, перемешайте. Влейте 500 мл горячей воды, накройте крышкой и тушите на слабом огне 20 минут. Выложите 300 г риса ровным слоем, воткните 30 г очищенного чеснока, влейте оставшиеся 400 мл горячей воды так, чтобы вода была выше риса примерно на 1 см. Доведите до кипения, накройте крышкой и готовьте на слабом огне 25 минут. Снимите с огня и дайте постоять под крышкой 10 минут. Перемешайте и подавайте.', 70);
INSERT INTO foodgram.recipes (id, author_id, name, image, text, cooking_time) VALUES (3, 1, 'Гречка с фаршем и овощами', 'http://localhost/media/recipes/ac1afbc435bc4ddb9f303376e595ad35.jpg', 'Промойте 200 г гречневой крупы. Лук (100 г) нарежьте кубиками, морковь (100 г) натрите на крупной тёрке. В кастрюле или глубокой сковороде разогрейте 30 мл растительного масла, обжарьте лук 2–3 минуты, добавьте морковь и обжаривайте ещё 3–4 минуты. Добавьте 300 г фарша, разбивая комки, и обжаривайте 5–7 минут до изменения цвета. Всыпьте 200 г гречки, добавьте 6 г соли и 1,5 г чёрного перца, перемешайте. Влейте 400 мл воды, доведите до кипения, накройте крышкой и готовьте на слабом огне 15–18 минут до впитывания воды. Снимите с огня и дайте постоять под крышкой 5 минут. Перемешайте и подавайте.', 40);
INSERT INTO foodgram.recipes (id, author_id, name, image, text, cooking_time) VALUES (4, 2, 'Курица в сливочном соусе со шпинатом', 'http://localhost/media/recipes/134c9cf097ae416b9532d155e91e3ba9.jpg', 'Куриное филе (300 г) нарежьте небольшими кусочками. Лук (150 г) и чеснок (20 г) мелко порубите. В сковороде разогрейте растительное масло (30 мл), пассеруйте лук и чеснок до прозрачности. Добавьте смесь специй (10 г), выложите курицу и обжаривайте до готовности. Добавьте шпинат (500 г), влейте сливки (250 мл), посолите (5 г), поперчите (1 г) и тушите на медленном огне 5 минут. Готовое блюдо посыпьте тёртым пармезаном (30 г) и кедровыми орешками (30 г).', 30);
INSERT INTO foodgram.recipes (id, author_id, name, image, text, cooking_time) VALUES (5, 2, 'Пате гурмэ', 'http://localhost/media/recipes/26e8dccaa79c46a2ab6781956b1ea086.jpg', 'Измельчите 150 г телятины, 100 г говядины, 250 г утиной печени и 300 г говяжьей печени в блендере до однородной массы. Добавьте 60 мл оливкового масла, 5 г соли, 1 г белого перца и 20 г рубленого базилика, тщательно перемешайте. Переложите массу в форму и отправьте в духовку, разогретую до 170°C. Выпекайте 30–40 минут до появления золотистой корочки. Перед подачей полностью охладите.', 50);
INSERT INTO foodgram.recipes (id, author_id, name, image, text, cooking_time) VALUES (6, 2, 'Паштет из куриной печени с кальвадосом', 'http://localhost/media/recipes/ac7ad460471d481fab59994ef8b80a73.jpg', 'В сотейнике растопите немного сливочного масла (из 200 г) и обжарьте мелко нарезанный лук-порей и чеснок около 10 минут до мягкости. Оставшееся масло нарежьте кубиками и уберите в холодильник. Добавьте в сотейник 100 мл кальвадоса и 200 мл сливок, доведите до кипения и варите на слабом огне до загустения. Добавьте 1000 г печени, постоянно помешивая, готовьте около 10 минут. Посолите (8 г), поперчите (2 г) и приправьте 2 г сушёного тимьяна. Взбейте печень блендером до консистенции мусса, постепенно добавляя охлаждённое масло (примерно 100 г). Разложите паштет по формочкам. Оставшееся масло растопите и полейте паштет. Сверху посыпьте тимьяном и 3 г смеси перцев горошком. Поставьте в холодильник минимум на 6 часов.', 30);
INSERT INTO foodgram.recipes (id, author_id, name, image, text, cooking_time) VALUES (7, 3, 'Рататуй', 'http://localhost/media/recipes/c078364d049c45d2aa8cb44237ad9a0d.jpg', 'Овощи вымыть. Баклажан и кабачок нарезать крупными кубиками, перец — кубиками. С помидоров снять кожицу и крупно нарезать. Лук и чеснок мелко порубить. В сковороде с разогретым маслом (40 мл) обжарить лук и половину чеснока 2–3 минуты. Положить баклажаны и обжаривать 3 минуты. Добавить перец и готовить 2 минуты. Положить кабачки и готовить ещё 2–3 минуты. Затем добавить помидоры, посолить (5 г), поперчить (1 г), накрыть крышкой и тушить на слабом огне 20–25 минут. Выключить огонь, добавить оставшийся чеснок и зелень (20 г), дать настояться 10 минут.', 40);
INSERT INTO foodgram.recipes (id, author_id, name, image, text, cooking_time) VALUES (8, 3, 'Телятина по-бургундски', 'http://localhost/media/recipes/09d51648a0c04e8da15fc7ebccff0979.jpg', 'Крупно нарежьте 1 луковицу, 1 морковь и 1 стебель порея. Смешайте с 2 г тимьяна, 3 лавровыми листьями, 1,5 г перца, 4 гвоздиками и 1 г базилика. Добавьте 800 г телятины крупными кусками и залейте 750 мл красного вина. Маринуйте в холодильнике 24 часа. Достаньте мясо, обсушите, маринад процедите. В сковороде разогрейте 40 г сливочного масла и 30 мл растительного, обжарьте телятину до румяной корочки. В другой сковороде обжарьте оставшийся лук (200 г) с 150 г бекона. Переложите в кастрюлю обжаренные овощи из маринада, мясо, бекон с луком, добавьте 30 г муки, влейте процеженный маринад, добавьте 6 г соли. Тушите на слабом огне 3,5 часа. За 1 час до готовности добавьте 600 г картофеля. За 20 минут — 300 г обжаренных шампиньонов и 1 г петрушки.', 270);
INSERT INTO foodgram.recipes (id, author_id, name, image, text, cooking_time) VALUES (9, 3, 'Фрикасе из куропатки', 'http://localhost/media/recipes/a56ae2fd35914882b01d4fc7f4ea78e0.png', 'Растопите 60 г масла в сотейнике, уложите куропаток и обжаривайте на медленном огне, пока мясо не станет сероватым. Выньте птицу. В оставшемся масле прожарьте муку (30 г) до бледно-жёлтого цвета. Снимите с огня, влейте бульон (570 мл), перемешайте, добавьте сок и цедру половины лимона. Верните на огонь, доведите до кипения и уменьшите огонь. Добавьте лук (200 г) и грибы (340 г), тушите 10 минут. Верните куропаток, добавьте пучок приправ (10 г), соль (6 г) и перец (1,5 г), тушите под крышкой 30–40 минут. В отдельной миске взбейте желтки (2 шт.) с молоком (45 мл) и сливками (90 мл). Влейте смесь в сотейник, прогрейте, не доводя до кипения. Посыпьте рубленой зеленью (10 г).', 60);
INSERT INTO foodgram.recipes (id, author_id, name, image, text, cooking_time) VALUES (10, 4, 'Бургер «И рыба и мясо»', 'http://localhost/media/recipes/d8bf219947de484f9618c3137d6fb83d.jpg', 'Из фарша (200 г) сформируйте котлету, посолите (2 г) и поперчите (1 г). Обжаривайте на разогретой сковороде по 3–4 минуты с каждой стороны. За минуту до готовности положите сверху сыр (30 г), чтобы он расплавился. Булочку разрежьте и подсушите на сухой сковороде. Соберите бургер: нижняя булочка, соус (30 г), салат (20 г), котлета с сыром, карамелизированный лук (50 г), ломтик помидора (30 г), верхняя булочка.', 20);
INSERT INTO foodgram.recipes (id, author_id, name, image, text, cooking_time) VALUES (11, 4, 'Свиные рёбра барбекю', 'http://localhost/media/recipes/d79eeb0a5fc140f7a59d580accea5b2d.jpg', 'Рёбра (500 г) обсушите, натрите солью (5 г), перцем (2 г) и копчёной паприкой (3 г). Смажьте растительным маслом (20 мл). Заверните в фольгу и запекайте в духовке при 160°C в течение 1 часа. Затем разверните, обильно смажьте соусом барбекю (100 г) и запекайте ещё 15–20 минут при 200°C до карамелизации глазури. Дайте немного остыть и подавайте.', 90);


--
-- Data for Name: favorite_recipes; Type: TABLE DATA; Schema: foodgram; Owner: -
--



--
-- Data for Name: purchases; Type: TABLE DATA; Schema: foodgram; Owner: -
--



--
-- Data for Name: recipe_ingredients; Type: TABLE DATA; Schema: foodgram; Owner: -
--

INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (1, 1, 1830, 250);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (2, 1, 1858, 100);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (3, 1, 1077, 150);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (4, 1, 660, 100);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (5, 1, 1162, 17);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (6, 1, 1170, 2);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (7, 2, 1450, 300);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (8, 2, 278, 500);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (9, 2, 1053, 300);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (10, 2, 886, 200);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (11, 2, 2055, 30);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (12, 2, 1419, 100);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (13, 2, 252, 900);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (14, 2, 1684, 12);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (15, 2, 440, 3);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (16, 2, 112, 6);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (17, 2, 1284, 2);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (18, 3, 347, 200);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (19, 3, 1913, 300);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (20, 3, 1053, 100);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (21, 3, 886, 100);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (22, 3, 252, 400);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (23, 3, 1419, 30);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (24, 3, 1684, 6);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (25, 3, 1284, 2);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (26, 4, 780, 300);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (27, 4, 2122, 500);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (28, 4, 1636, 250);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (29, 4, 886, 150);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (30, 4, 2055, 20);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (31, 4, 1419, 30);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (32, 4, 1210, 30);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (33, 4, 583, 30);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (34, 4, 1684, 5);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (35, 4, 1284, 1);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (36, 5, 1818, 150);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (37, 5, 278, 100);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (38, 5, 1904, 250);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (39, 5, 293, 300);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (40, 5, 1162, 60);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (41, 5, 79, 20);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (42, 5, 1684, 5);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (43, 5, 1238, 1);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (44, 6, 778, 1000);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (45, 6, 884, 150);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (46, 6, 2055, 20);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (47, 6, 1646, 200);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (48, 6, 504, 100);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (49, 6, 1636, 200);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (50, 6, 1849, 2);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (51, 6, 1684, 8);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (52, 6, 1284, 2);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (53, 7, 83, 300);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (54, 7, 2019, 200);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (55, 7, 1240, 200);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (56, 7, 1341, 300);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (57, 7, 886, 150);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (58, 7, 2055, 10);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (59, 7, 1162, 40);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (60, 7, 1684, 5);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (61, 7, 1284, 1);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (62, 7, 435, 20);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (63, 8, 1818, 800);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (64, 8, 232, 750);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (65, 8, 556, 600);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (66, 8, 2077, 300);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (67, 8, 120, 150);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (68, 8, 1053, 300);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (69, 8, 884, 200);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (70, 8, 886, 200);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (71, 8, 1646, 40);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (72, 8, 1419, 30);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (73, 8, 1081, 30);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (74, 8, 1284, 2);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (75, 8, 1849, 2);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (76, 8, 816, 3);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (77, 8, 266, 4);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (78, 8, 80, 1);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (79, 8, 1684, 6);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (80, 8, 1310, 1);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (81, 9, 804, 800);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (82, 9, 1646, 60);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (83, 9, 1081, 30);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (84, 9, 794, 570);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (85, 9, 886, 200);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (86, 9, 353, 340);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (87, 9, 855, 100);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (88, 9, 2175, 50);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (89, 9, 1032, 45);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (90, 9, 1638, 90);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (91, 9, 1305, 10);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (92, 9, 1684, 6);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (93, 9, 1284, 2);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (94, 10, 156, 80);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (95, 10, 2037, 30);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (96, 10, 878, 50);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (97, 10, 1692, 30);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (98, 10, 1522, 20);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (99, 10, 1341, 30);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (100, 10, 1684, 2);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (101, 10, 1284, 1);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (102, 10, 280, 200);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (103, 11, 1599, 500);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (104, 11, 1695, 100);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (105, 11, 1684, 5);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (106, 11, 1284, 2);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (107, 11, 1203, 3);
INSERT INTO foodgram.recipe_ingredients (id, recipe_id, ingredient_id, amount) VALUES (108, 11, 1419, 20);


--
-- Data for Name: subscriptions; Type: TABLE DATA; Schema: foodgram; Owner: -
--



--
-- Name: favorite_recipes_id_seq; Type: SEQUENCE SET; Schema: foodgram; Owner: -
--

SELECT pg_catalog.setval('foodgram.favorite_recipes_id_seq', 1, false);


--
-- Name: ingredients_id_seq; Type: SEQUENCE SET; Schema: foodgram; Owner: -
--

SELECT pg_catalog.setval('foodgram.ingredients_id_seq', 2186, true);


--
-- Name: purchases_id_seq; Type: SEQUENCE SET; Schema: foodgram; Owner: -
--

SELECT pg_catalog.setval('foodgram.purchases_id_seq', 1, false);


--
-- Name: recipe_ingredients_id_seq; Type: SEQUENCE SET; Schema: foodgram; Owner: -
--

SELECT pg_catalog.setval('foodgram.recipe_ingredients_id_seq', 108, true);


--
-- Name: recipes_id_seq; Type: SEQUENCE SET; Schema: foodgram; Owner: -
--

SELECT pg_catalog.setval('foodgram.recipes_id_seq', 11, true);


--
-- Name: subscriptions_id_seq; Type: SEQUENCE SET; Schema: foodgram; Owner: -
--

SELECT pg_catalog.setval('foodgram.subscriptions_id_seq', 1, false);


--
-- Name: users_id_seq; Type: SEQUENCE SET; Schema: foodgram; Owner: -
--

SELECT pg_catalog.setval('foodgram.users_id_seq', 4, true);


--
-- PostgreSQL database dump complete
--

\unrestrict cTV3y4pDbzL4EPZaCxKRrnNqo7vfOu40UGMdx9cNXy6JOpFHuFocZboYozyJNIT

