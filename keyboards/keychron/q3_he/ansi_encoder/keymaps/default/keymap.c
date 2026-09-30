void housekeeping_task_user(void) {

    if (!macro_activo) {
        return;
    }

    uint32_t ahora = timer_read32();

    // Liberación asíncrona de Y
    if (macro_y_activo && timer_expired32(ahora, macro_fin_y)) {
        unregister_code(KC_Y);
        macro_y_activo = false;
        macro_fin_y = 0;
    }

    // Liberación asíncrona de C
    if (macro_c_activo && timer_expired32(ahora, macro_fin_c)) {
        unregister_code(KC_C);
        macro_c_activo = false;
        macro_fin_c = 0;
    }

    // Liberación asíncrona de L
    if (macro_l_activo && timer_expired32(ahora, macro_fin_l)) {
        unregister_code(KC_L);
        macro_l_activo = false;
        macro_fin_l = 0;
    }

    // Liberación asíncrona de Home
    if (macro_home_activo && timer_expired32(ahora, macro_fin_home)) {
        unregister_code(KC_HOME);
        macro_home_activo = false;
        macro_fin_home = 0;
    }

    if (!timer_expired32(ahora, macro_proximo_evento)) {
        return;
    }

    switch (macro_estado) {

        /* ----------------------------------------------------
         * 0. INICIO ROTACIÓN -> HOME -> ESPERA RÁPIDA -> F1
         * ---------------------------------------------------- */
        case 0:
            register_code(KC_HOME);
            macro_home_activo = true;
            // Pulsación rápida de Home (30 ms a 50 ms)
            macro_fin_home = ahora + 30 + (rand() % 21);

            // Programar el siguiente bucle de HOME (1m 40s a 1m 42s)
            macro_proximo_home = ahora + 100000 + (rand() % 2001);

            macro_proximo_evento = macro_fin_home;
            macro_estado = 1;
            break;

        case 1:
            unregister_code(KC_HOME);
            macro_home_activo = false;
            macro_fin_home = 0;

            // Espera rápida antes de F1 (80 ms a 150 ms)
            macro_proximo_evento = ahora + 80 + (rand() % 71);
            macro_estado = 101;
            break;

        case 101:
            register_code(KC_F1);
            macro_proximo_evento = ahora + 80 + (rand() % 31);

            // C agendada para 60-63s tras F1
            macro_proximo_c = ahora + 60000 + (rand() % 3001);

            macro_estado = 2;
            break;

        case 2:
            unregister_code(KC_F1);
            macro_proximo_evento = ahora + 1000 + (rand() % 1001);
            macro_estado = 3;
            break;

        case 3:
            register_code(KC_Y);
            macro_y_activo = true;
            macro_fin_y = ahora + 100 + (rand() % 41);

            macro_proximo_y = ahora + 60000 + (rand() % 3001);

            macro_proximo_evento = macro_fin_y;
            macro_estado = 4;
            break;

        case 4:
            unregister_code(KC_Y);
            macro_y_activo = false;
            macro_fin_y = 0;

            macro_proximo_sws = ahora;

            macro_proximo_evento = ahora + 900 + (rand() % 201);
            macro_estado = 10;
            break;

        /* ====================================================
         * BUCLE PRINCIPAL
         * ==================================================== */

        case 10:
            // 0. Revisar si toca la tecla Home (cada 1m 40s a 1m 42s)
            if (macro_proximo_home != 0 && timer_expired32(ahora, macro_proximo_home)) {
                register_code(KC_HOME);
                macro_home_activo = true;
                // Pulsación rápida de Home en bucle (30 ms a 50 ms)
                macro_fin_home = ahora + 30 + (rand() % 21);
                macro_proximo_home = ahora + 100000 + (rand() % 2001);
                macro_proximo_evento = macro_fin_home;
                macro_estado = 17;
                break;
            }

            // 1. Revisar si toca la tecla L (3 pulsaciones distribuidas)
            if (macro_proximo_l != 0 && timer_expired32(ahora, macro_proximo_l)) {
                register_code(KC_L);
                macro_l_activo = true;
                macro_fin_l = ahora + 100 + (rand() % 41);

                macro_conteo_l++;

                if (macro_conteo_l < 3) {
                    macro_proximo_l = ahora + 35000 + (rand() % 4001);
                } else {
                    macro_proximo_l = 0;
                }

                macro_proximo_evento = macro_fin_l;
                macro_estado = 16;
                break;
            }

            // 2. Revisar si toca la tecla Y (cada 60-63s)
            if (macro_proximo_y != 0 && timer_expired32(ahora, macro_proximo_y)) {
                register_code(KC_Y);
                macro_y_activo = true;
                macro_fin_y = ahora + 100 + (rand() % 41);
                macro_proximo_y = ahora + 60000 + (rand() % 3001);
                macro_proximo_evento = macro_fin_y;
                macro_estado = 14;
                break;
            }

            // 3. Revisar si toca la tecla C (60-63s tras F1)
            if (macro_proximo_c != 0 && timer_expired32(ahora, macro_proximo_c)) {
                register_code(KC_C);
                macro_c_activo = true;
                macro_fin_c = ahora + 100 + (rand() % 41);
                macro_proximo_c = ahora + 60000 + (rand() % 3001);
                macro_proximo_evento = macro_fin_c;
                macro_estado = 15;
                break;
            }

            // 4. Revisar si toca Shift + W + S (Cada 7s)
            if (macro_proximo_sws != 0 && timer_expired32(ahora, macro_proximo_sws)) {
                register_code(KC_LSFT);
                macro_proximo_evento = ahora + 120 + (rand() % 35);
                macro_estado = 20;
                break;
            }

            // 5. Shift + W normal
            register_code(KC_LSFT);
            macro_proximo_evento = ahora + 120 + (rand() % 35);
            macro_estado = 11;
            break;

        case 11:
            unregister_code(KC_LSFT);
            macro_proximo_evento = ahora + 800 + (rand() % 201);
            macro_estado = 12;
            break;

        case 12:
            register_code(KC_W);
            macro_proximo_evento = ahora + 120 + (rand() % 35);
            macro_estado = 13;
            break;

        case 13:
            unregister_code(KC_W);

            macro_combos_realizados++;
            if (macro_conteo_l == 0 && macro_combos_realizados >= 2) {
                macro_proximo_l = ahora + 500 + (rand() % 1001);
            }

            if (timer_expired32(ahora, macro_fin_rotacion)) {
                unregister_code(KC_LSFT);
                unregister_code(KC_W);
                unregister_code(KC_S);

                if (macro_rotacion >= 9) {
                    macro_cancelar();
                    return;
                }

                macro_proximo_evento = ahora + 1500 + (rand() % 1001);
                macro_estado = 100;
                return;
            }

            macro_proximo_evento = ahora + 900 + (rand() % 201);
            macro_estado = 10;
            break;

        case 20:
            unregister_code(KC_LSFT);
            macro_proximo_evento = ahora + 400 + (rand() % 101);
            macro_estado = 21;
            break;

        case 21:
            register_code(KC_W);
            macro_proximo_evento = ahora + 120 + (rand() % 35);
            macro_estado = 22;
            break;

        case 22:
            unregister_code(KC_W);
            macro_proximo_evento = ahora + 200 + (rand() % 101);
            macro_estado = 23;
            break;

        case 23:
            register_code(KC_S);
            macro_proximo_evento = ahora + 120 + (rand() % 35);
            macro_estado = 24;
            break;

        case 24:
            unregister_code(KC_S);

            macro_proximo_sws = ahora + 7000 + (rand() % 501);

            if (timer_expired32(ahora, macro_fin_rotacion)) {
                unregister_code(KC_LSFT);
                unregister_code(KC_W);
                unregister_code(KC_S);

                if (macro_rotacion >= 9) {
                    macro_cancelar();
                    return;
                }

                macro_proximo_evento = ahora + 1500 + (rand() % 1001);
                macro_estado = 100;
                return;
            }

            macro_proximo_evento = ahora + 900 + (rand() % 201);
            macro_estado = 10;
            break;

        case 14:
            unregister_code(KC_Y);
            macro_y_activo = false;
            macro_fin_y = 0;
            macro_proximo_evento = ahora + 900 + (rand() % 201);
            macro_estado = 10;
            break;

        case 15:
            unregister_code(KC_C);
            macro_c_activo = false;
            macro_fin_c = 0;
            macro_proximo_evento = ahora + 900 + (rand() % 201);
            macro_estado = 10;
            break;

        case 16:
            unregister_code(KC_L);
            macro_l_activo = false;
            macro_fin_l = 0;
            macro_proximo_evento = ahora + 900 + (rand() % 201);
            macro_estado = 10;
            break;

        case 17:
            unregister_code(KC_HOME);
            macro_home_activo = false;
            macro_fin_home = 0;

            // Retorno rápido al bucle principal (100 ms a 200 ms)
            macro_proximo_evento = ahora + 100 + (rand() % 101);
            macro_estado = 10;
            break;

        case 100:
            macro_nueva_rotacion();
            break;
    }
}
